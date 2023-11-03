/*
** EPITECH PROJECT, 2023
** ecs
** File description:
** ServerImpl.cpp
*/

#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include "../include/Server.hpp"

#define DEBUG

namespace serv {
    std::unique_ptr<ServerImpl> ServerImpl::_instance = nullptr;

    ServerImpl::ServerImpl()
    {
    }

    ServerImpl::~ServerImpl()
    {
    }

    void ServerImpl::Start()
    {
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1)
            throw std::runtime_error("Failed to create socket.");

        int opt = 1;
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
            throw std::runtime_error("Failed to set socket options.");

        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(_port);
        if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
            throw std::runtime_error("Failed to bind socket to port.");

        if (listen(_socket, _maxClients) < 0)
            throw std::runtime_error("Failed to listen for connections.");

        #ifdef DEBUG
        std::cout << "Server listening on port " << _port << std::endl;
        #endif
    }

    ServerImpl *ServerImpl::Get()
    {
        if (_instance == nullptr)
            _instance = std::make_unique<ServerImpl>();
        return _instance.get();
    }

    void ServerImpl::BufferMessage(const std::string &data, int socket)
    {
        if (_bufferedMessages.find(socket) == _bufferedMessages.end()) {
            _bufferedMessages[socket] = std::queue<std::string>();
        }
        _bufferedMessages[socket].push(data);
    }

    void ServerImpl::SetPort(int port)
    {
        _port = port;
    }

    void ServerImpl::SetMaxClients(int maxClients)
    {
        _maxClients = maxClients;
    }

    void ServerImpl::AcceptNewClient()
    {
        if (_maxClients > 0 && _clients.size() >= _maxClients)
            return;
        int newSocket = accept(_socket, (struct sockaddr *)&_address, (socklen_t *)&_addrlen);
        if (newSocket < 0)
            throw std::runtime_error("Failed to accept new client.");

        int flags = fcntl(newSocket, F_GETFL, 0);
        if (flags < 0)
            throw std::runtime_error("Failed to get socket flags.");

        if (fcntl(newSocket, F_SETFL, flags | O_NONBLOCK) < 0)
            throw std::runtime_error("Failed to set socket flags.");

        _clients.emplace_back(newSocket);
        _clients.back() << Message::WaitingForHandshake;
        std::cout << "New client connected, waiting for handshake." << std::endl;
    }

    void ServerImpl::Poll()
    {
        std::vector<pollfd> pollfds;
        pollfds.emplace_back(pollfd{.fd = _socket, .events = POLLIN, .revents = 0});
        for (auto &client : _clients) {
            pollfds.emplace_back(pollfd{.fd = client.GetSocket(), .events = POLLIN, .revents = 0});
        }

        int timeout = 100;
        int ret = poll(pollfds.data(), pollfds.size(), timeout);
        if (ret < 0)
            throw std::runtime_error("Failed to poll sockets.");

        if (pollfds[0].revents & POLLIN)
            AcceptNewClient();

        std::queue<int> toRemove;
        for (int i = 1; i < pollfds.size(); ++i) {
            if (pollfds[i].revents & POLLIN) {
                char buffer[1024] = {0};
                int valread = read(pollfds[i].fd, buffer, 1024);
                if (valread == 0) {
                    std::cout << "Client disconnected." << std::endl;
                    toRemove.push(i - 1);
                    continue;
                }
                if (not checkForHandshake(_clients[i - 1], std::string(buffer)))
                    _clients[i - 1].Receive(buffer);
            }
        }

        while (!toRemove.empty()) {
            _clients.erase(_clients.begin() + toRemove.front());
            toRemove.pop();
        }

        SendBufferedMessages();
    }

    bool ServerImpl::checkForHandshake(ClientBucket &client, const std::string &data)
    {
        std::cout << "data is [" << data << "]" << std::endl;
        if (data.find("Handshake: ") == std::string::npos)
            return false;
        std::cout << "contains handshake"<< std::endl;
        std::string handshake = data.substr(data.find("Handshake: ") + 11);
        std::cout << "substring is [" << handshake << "]" << std::endl;




        if (_handshakes.find(handshake) == _handshakes.end()) {
            client << Message("Handshake not found.", 1);
            return true;
        }
        client.SetClient(_handshakes[handshake]());
        client << Message("Handshake OK.", 0);
        return true;
    }

    void ServerImpl::SendBufferedMessages()
    {
        for (auto &client : _clients) {
            int socket = client.GetSocket();
            while (!_bufferedMessages[socket].empty()) {
                try {
                    std::string message = _bufferedMessages[socket].front();
                    send(socket, message.c_str(), message.size(), 0);
                    _bufferedMessages[socket].pop();
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
    }

    ServerImpl::HandleRequestException::HandleRequestException(const std::string &message, int code) :
        _message(message), _code(code)
    {
    }

    const char *ServerImpl::HandleRequestException::what() const noexcept
    {
        return _message.c_str();
    }
}
