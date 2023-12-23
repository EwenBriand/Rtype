/*
** EPITECH PROJECT, 2023
** server
** File description:
** ClientUDP.cpp
*/

#include "ClientUDP.hpp"
#include "ServerUdp.hpp"
#include <iostream>

namespace serv {
    ClientUDP::ClientUDP()
        : _socket(_ioService)
        , _inBuffer(BUFF_SIZE)
        , _mutex(std::make_shared<std::mutex>())
        , _running(false)
    {
    }

    ClientUDP::~ClientUDP()
    {
        _running = false;
        if (_sendThread.joinable())
            _sendThread.join();
        if (_receiveThread.joinable())
            _receiveThread.join();
    }

    void ClientUDP::SetRequestHandler(std::shared_ptr<IClientRequestHandler> requestHandler)
    {
        _requestHandler = requestHandler;
    }

    void ClientUDP::Send(const bytes& data)
    {
        _sendQueue.Push(data);
    }

    void ClientUDP::Send(const Instruction& instruction)
    {
        _sendQueue.Push(instruction.ToBytes());
    }

    void ClientUDP::SetServerAddress(const std::string& ip, int port)
    {
        _serverIp = ip;
        _serverPort = port;
    }

    void ClientUDP::sendWorker()
    {
        while (_running) {
            try {
                bytes data = _sendQueue.Pop();
                _socket.send_to(boost::asio::buffer(data._data), boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(_serverIp), _serverPort));
            } catch (std::exception& e) {
                // empty queue
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void ClientUDP::receiveWorker()
    {
        while (_running) {
            try {
                boost::asio::ip::udp::endpoint senderEndpoint;
                bytes data;
                data.resize(1024);
                std::size_t bytesTransferred = _socket.receive_from(boost::asio::buffer(data._data), senderEndpoint);
                data.resize(bytesTransferred);
                {
                    std::lock_guard<std::mutex> lock(*_mutex);
                    _inBuffer.Write(bytes(data));
                }

            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void ClientUDP::Start()
    {
        if (_requestHandler == nullptr)
            throw std::runtime_error("Request handler not set");
        _running = true;
        _socket.open(boost::asio::ip::udp::v4());
        _sendThread = std::thread(&ClientUDP::sendWorker, this);
        _receiveThread = std::thread(&ClientUDP::receiveWorker, this);
    }

    void ClientUDP::Stop()
    {
        _running = false;
    }

    void ClientUDP::CallHook()
    {
        while (!_inBuffer.IsEmpty()) {
            bytes data;
            {
                std::lock_guard<std::mutex> lock(*_mutex);
                data = _inBuffer.Read();
            }
            if (_requestHandler != nullptr and !data.empty())
                _requestHandler->HandleRequest(data);
        }
    }
}