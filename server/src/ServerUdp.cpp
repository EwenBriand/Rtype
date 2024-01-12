/*
** EPITECH PROJECT, 2023
** server
** File description:
** ServerUdp.cpp
*/

#include "ServerUdp.hpp"
// #include "Engine.hpp"
#include "NetworkExceptions.hpp"
#include "AsioClone.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <memory>

namespace serv {

    // ============================================================================
    // ACLIENT
    // ============================================================================

    AClient::AClient(ServerUDP& server)
        : _server(server), _endpoint(std::make_shared<EndpointWrapper>())
    {
    }

    void AClient::SetEndpoint(std::shared_ptr<EndpointWrapper> endpoint)
    {
        _endpoint = endpoint;
    }

    void AClient::ResetAnswerFlag()
    {
        _answerFlag = false;
    }

    bool AClient::GetAnswerFlag()
    {
        return _answerFlag;
    }

    // ============================================================================
    // CLIENT BUCKET
    // ============================================================================

    ClientBucketUDP::ClientBucketUDP(std::shared_ptr<EndpointWrapper> endpoint)
        : _endpoint(new EndpointWrapper(*endpoint))
        , _clientHandler(nullptr)
        // , _buffer(2 * BUFF_SIZE)
        , _mutex(std::make_shared<std::mutex>())
        , _lastRequestTimeMutex(std::make_shared<std::mutex>())
        , _lastRequestTime(std::chrono::high_resolution_clock::now())
    {
    }

    ClientBucketUDP::~ClientBucketUDP()
    {
    }

    void ClientBucketUDP::UpdateLastRequestTime()
    {
        std::scoped_lock lock(*_lastRequestTimeMutex);
        _lastRequestTime = std::chrono::high_resolution_clock::now();
    }

    float ClientBucketUDP::GetLastRequestTime()
    {
        std::scoped_lock lock(*_lastRequestTimeMutex);
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _lastRequestTime).count();
    }

    void ClientBucketUDP::Write(const bytes& data)
    {
        std::scoped_lock lock(*_mutex);
        // _buffer.Write(data);
        _buffer.Push(data);
    }

    bytes ClientBucketUDP::Read()
    {
        bytes value;
        {
            std::scoped_lock lock(*_mutex);
            // value = _buffer.ReadUntil(SEPARATOR);
            try {
                value = _buffer.Pop();
            } catch (const std::exception& e) {
                return bytes();
            }
        }
        return value;
    }

    std::shared_ptr<EndpointWrapper> ClientBucketUDP::GetEndpoint() const
    {
        return _endpoint;
    }

    void ClientBucketUDP::HandleRequest(ServerUDP& server)
    {
        bytes data = Read();

        while (not data.empty()) {
            data.resize(data.size() - SEPARATOR.size());
            Instruction instruction(data);
            if (server.GetInterceptors().find(instruction.opcode) != server.GetInterceptors().end()) {
                server.GetInterceptors().at(instruction.opcode)(instruction, _endpoint);
                std::cout << "instruction intercepted" << std::endl;
                data = Read();
                continue;
            }
            // std::cout << "\rHandleRequest: " << data.toString() << std::endl;
            _clientHandler->HandleRequest(data);
            data = Read();
        }
    }

    void ClientBucketUDP::SetHandleRequest(std::shared_ptr<IClient> clientHandler)
    {
        _clientHandler = clientHandler;
        _clientHandler->SetEndpoint(_endpoint);
    }

    void ClientBucketUDP::OnDisconnect()
    {
        if (_clientHandler != nullptr)
            _clientHandler->OnDisconnect();
    }

    // ============================================================================
    // SERVER UDP
    // ============================================================================

    ServerUDP::ServerUDP(unsigned short port)
        : _asio(std::make_shared<AsioClone>())
        , _clientHandlerCopyBase(nullptr)
        , _running(false)
        , _logMutex(std::make_shared<std::mutex>())
        , _clientsMutex(std::make_shared<std::mutex>())
    {
        _endpoint = std::make_shared<EndpointWrapper>(boost::asio::ip::udp::v4(), port);
        _remoteEndpoint = std::make_shared<EndpointWrapper>();
        AsioClone::error_code error;
        _asio->open(_endpoint->endpoint.protocol(), error);
        if (error)
            throw std::runtime_error("Failed to open socket (1)");
        _asio->set_option_reuseaddr();
        _asio->bind(*_endpoint, error);
        if (error)
            throw std::runtime_error("Failed to open socket (2)");
        std::cout << "Socket opened" << std::endl;
        try {
            if (std::filesystem::exists(".server.log"))
                std::filesystem::remove(".server.log");
            _logFile = std::ofstream(".server.log");
            if (!_logFile.is_open())
                std::cerr << "Failed to open log file" << std::endl;
            else {
                std::cout << "Server log in .server.log" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    ServerUDP::~ServerUDP()
    {
        _running = false;

        if (_receiveThread.joinable())
            _receiveThread.join();
        if (_sendThread.joinable())
            _sendThread.join();
        if (_checkForDisconnectionsThread.joinable())
            _checkForDisconnectionsThread.join();
        _asio->stop();
    }

    void ServerUDP::Start()
    {
        if (_clientHandlerCopyBase == nullptr)
            throw std::runtime_error("No client handler set");
        _running = true;
        _asio->run();
        _receiveThread
            = std::thread(&ServerUDP::receiveWorker, this);
        _sendThread = std::thread(&ServerUDP::sendWorker, this);
        _checkForDisconnectionsThread = std::thread(&ServerUDP::checkForDisconnections, this);
    }

    void ServerUDP::receiveWorker()
    {
        while (_running) {
            _asio->receive_from(_buffer, _remoteEndpoint->endpoint);
            std::cout << "Received from endpoint: " << endpointToString(_remoteEndpoint) << std::endl;
            std::size_t bytesTransferred = bytes::find_last_of(_buffer, [](unsigned char c) {
                return c != 0;
            });
            bytesTransferred += 1;
            int opcode = Instruction(bytes(_buffer.data(), bytesTransferred)).opcode;
            if (opcode != I_AM_ALIVE)
                Log("Received opcode " + std::to_string(opcode) + " from " + endpointToString(_remoteEndpoint) + " (" + std::to_string(bytesTransferred) + " bytes) " + bytes(_buffer.data(), bytesTransferred).toString());
            HandleRequest(bytesTransferred);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void ServerUDP::HandleRequest(std::size_t bytesTransferred)
    {
        AsioClone::error_code error = _asio->init_error_code();
        if (error && error != _asio->get_message_size_error())
            return;
        bytes data(_buffer.data(), bytesTransferred);

        try {
            passToClient(data);
        } catch (const std::exception& e) {
            Log(e.what());
            if (_clients.find(endpointToString(_remoteEndpoint)) != _clients.end())
                _clients.erase(endpointToString(_remoteEndpoint));
        }
    }

    void ServerUDP::passToClient(bytes& data)
    {
        std::string clientKey = endpointToString(_remoteEndpoint);
        std::scoped_lock lock(*_clientsMutex);
        if (_clients.find(clientKey) == _clients.end()) {
            _clients.insert({ clientKey, std::make_shared<ClientBucketUDP>(_remoteEndpoint) });
            _clients.at(clientKey)->SetHandleRequest(_clientHandlerCopyBase->Clone(_remoteEndpoint));
            Log("New client connected: " + clientKey);
        }
        _clients.at(clientKey)->UpdateLastRequestTime();
        _clients.at(clientKey)->Write(data);
    }

    void ServerUDP::checkForDisconnections()
    {
        std::vector<std::string> disconnectedClients;
        while (_running) {
            for (auto& client : _clients) {
                if (client.second->GetLastRequestTime() > 3000) {
                    std::string clientKey = endpointToString(client.second->GetEndpoint());
                    Log("Client " + clientKey + " disconnected");
                    _clients.at(clientKey)->OnDisconnect();
                    disconnectedClients.push_back(clientKey);
                    Broadcast(Instruction(I_MESSAGE, 0, bytes("Client disconnected")).ToBytes() + SEPARATOR);
                }
            }
            {
                std::scoped_lock lock(*_clientsMutex);
                for (auto& clientKey : disconnectedClients) {
                    _clients.erase(clientKey);
                }
                if (_resetClientsFlag) {
                    _clients.clear();
                    _resetClientsFlag = false;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void ServerUDP::ResetClients()
    {
        _resetClientsFlag = true;
    }

    void ServerUDP::sendWorker()
    {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        while (_running) {
            if (!_sendQueue.IsEmpty()) {
                try {
                    Message message = _sendQueue.Pop();
                    _asio->send_to(message.data._data, message.endpointW->endpoint);
                    std::cout << "Send to endpoint " << endpointToString(message.endpointW) << " with opcode " << std::to_string(Instruction(message.data).opcode) << std::endl;
                    Log("Sent opcode " + std::to_string(Instruction(message.data).opcode) + " to " + endpointToString(message.endpointW));
                } catch (std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void ServerUDP::Send(const Message& message)
    {
        Message messageCopy = message;
        _sendQueue.Push(messageCopy);
    }

    void ServerUDP::Send(const Instruction& instruction, std::shared_ptr<EndpointWrapper> endpoint)
    {
        _sendQueue.Push(instruction.ToMessage(*endpoint));
    }

    std::string ServerUDP::endpointToString(std::shared_ptr<EndpointWrapper> endpoint)
    {
        return endpoint->endpoint.address().to_string() + ":" + std::to_string(endpoint->endpoint.port());
    }

    void ServerUDP::Broadcast(const bytes& data)
    {
        for (auto& client : _clients) {
            Send(Message({ data, client.second->GetEndpoint() }));
        }
    }

    void ServerUDP::CallHooks()
    {
        std::scoped_lock lock(*_clientsMutex);
        for (auto& client : _clients) {
            client.second->HandleRequest(*this);
        }
    }

    std::string ServerUDP::bytesToString(const bytes& bytes)
    {
        std::stringstream ss;

        for (auto& byte : bytes) {
            if (std::isalnum(byte))
                ss << byte;
            else {
                ss << " \\x"
                   << std::setfill('0') << std::setw(2)
                   << std::hex << static_cast<int>(byte)
                   << " ";
            }
        }
        return ss.str();
    }

    bytes ServerUDP::stringToBytes(const std::string& str)
    {
        bytes bytes;
        for (auto& c : str)
            bytes.push_back(c);
        return bytes;
    }

    void ServerUDP::Log(const std::string& entry)
    {
        if (_logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            auto fractional_seconds = now - std::chrono::system_clock::from_time_t(in_time_t);

            std::scoped_lock lock(*_logMutex);
            _logFile << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d - %X")
                     << '.' << std::setfill('0') << std::setw(3)
                     << std::chrono::duration_cast<std::chrono::milliseconds>(fractional_seconds).count()
                     << " " << entry << std::endl;
        }
    }

    void ServerUDP::Broadcast(const Instruction& instruction)
    {
        for (auto& client : _clients) {
            Send(instruction, client.second->GetEndpoint());
        }
    }

    void ServerUDP::Intercept(int opcode, std::function<void(const Instruction&, boost::asio::ip::udp::endpoint& endpoint)> callback)
    {
        _interceptors.insert({ opcode, callback });
    }

    void ServerUDP::FeedMessage(Message& message)
    {
        _remoteEndpoint = message.endpoint;
        passToClient(message.data);
    }

    std::map<int, std::function<void(const Instruction&, boost::asio::ip::udp::endpoint& endpoint)>>& ServerUDP::GetInterceptors()
    {
        return _interceptors;
    }
}
