/*
** EPITECH PROJECT, 2023
** server
** File description:
** ServerUdp.cpp
*/

#include "ServerUdp.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace serv {

    // ============================================================================
    // CLIENT BUCKET
    // ============================================================================

    ClientBucketUDP::ClientBucketUDP(boost::asio::ip::udp::endpoint endpoint)
        : _endpoint(endpoint)
        , _clientHandler(nullptr)
        , _buffer(2 * BUFF_SIZE)
        , _mutex(std::make_shared<std::mutex>())
    {
    }

    ClientBucketUDP::~ClientBucketUDP()
    {
    }

    void ClientBucketUDP::Write(const bytes& data)
    {
        std::scoped_lock lock(*_mutex);
        _buffer.Write(data);
    }

    bytes ClientBucketUDP::Read()
    {
        bytes value;
        {
            std::scoped_lock lock(*_mutex);
            value = _buffer.ReadUntil(SEPARATOR);
        }
        if (value.size() >= SEPARATOR.size() && value.compare(value.size() - SEPARATOR.size(), SEPARATOR.size(), SEPARATOR) == 0)
            value.erase(value.size() - SEPARATOR.size(), SEPARATOR.size());
        return value;
    }

    boost::asio::ip::udp::endpoint ClientBucketUDP::GetEndpoint() const
    {
        return _endpoint;
    }

    void ClientBucketUDP::HandleRequest(ServerUDP& server)
    {
        bytes data = Read();

        while (not data.empty()) {
            bytes response = _clientHandler->HandleRequest(data);
            server.Send({ response, _endpoint });
            data = Read();
        }
    }

    void ClientBucketUDP::SetHandleRequest(std::shared_ptr<IClient> clientHandler)
    {
        _clientHandler = clientHandler;
    }

    // ============================================================================
    // SERVER UDP
    // ============================================================================i

    ServerUDP::ServerUDP(unsigned short port)
        : _socket(_ioService)
        , _endpoint(boost::asio::ip::udp::v4(), port)
        , _clientHandlerCopyBase(nullptr)
        , _running(false)
    {
        boost::system::error_code error;
        _socket.open(_endpoint.protocol(), error);
        if (error)
            throw std::runtime_error("Failed to open socket (1)");
        _socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        _socket.bind(_endpoint, error);
        if (error)
            throw std::runtime_error("Failed to open socket (2)");
    }

    ServerUDP::~ServerUDP()
    {
        _running = false;

        if (_receiveThread.joinable())
            _receiveThread.join();
        if (_sendThread.joinable())
            _sendThread.join();

        _ioService.stop();
    }

    void ServerUDP::Start()
    {
        if (_clientHandlerCopyBase == nullptr)
            throw std::runtime_error("No client handler set");
        _running = true;
        _ioService.run();
        _receiveThread
            = std::thread(&ServerUDP::receiveWorker, this);
        _sendThread = std::thread(&ServerUDP::sendWorker, this);
    }

    void ServerUDP::receiveWorker()
    {
        while (_running) {
            _socket.receive_from(
                boost::asio::buffer(_buffer), _remoteEndpoint);

            std::size_t bytesTransferred = bytes::find_last_of(_buffer, [](unsigned char c) {
                return c != 0;
            });
            bytesTransferred += 1;
            HandleRequest(boost::system::error_code(), bytesTransferred);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void ServerUDP::HandleRequest(
        const boost::system::error_code& error, std::size_t bytesTransferred)
    {
        if (error && error != boost::asio::error::message_size)
            return;
        bytes data(_buffer.data(), bytesTransferred);

        std::string clientKey = endpointToString(_remoteEndpoint);
        if (_clients.find(clientKey) == _clients.end()) {
            _clients.insert({ clientKey, std::make_shared<ClientBucketUDP>(_remoteEndpoint) });
            _clients.at(clientKey)->SetHandleRequest(_clientHandlerCopyBase->Clone());
        }
        _clients.at(clientKey)->Write(data);
    }

    void ServerUDP::sendWorker()
    {
        while (_running) {
            if (!_sendQueue.IsEmpty()) {
                try {
                    Message message = _sendQueue.Pop();
                    _socket.send_to(boost::asio::buffer(message.data._data), message.endpoint);
                } catch (std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void ServerUDP::Send(const Message& message)
    {
        _sendQueue.Push(message);
    }

    void ServerUDP::Send(const Instruction& instruction)
    {
        _sendQueue.Push(instruction.ToMessage());
    }

    std::string ServerUDP::endpointToString(boost::asio::ip::udp::endpoint endpoint)
    {
        return endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
    }

    void ServerUDP::Broadcast(const bytes& data)
    {
        for (auto& client : _clients) {
            client.second->Write(data);
        }
    }

    void ServerUDP::CallHooks()
    {
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
}
