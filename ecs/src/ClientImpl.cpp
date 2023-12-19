/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Client.cpp
*/
#include "ClientImpl.hpp"
#include "ECSImpl.hpp"
#include "Poll.hpp"
#include <cstring>
#include <future>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace serv {
    ClientImpl::ClientImpl(int timeout)
        : _running(true)
        , _timeout(timeout)
        , _bufferIn(10000)
        , _bufferOut(10000)
    {
    }

    ClientImpl::~ClientImpl()
    {
        Disconnect();
    }

    void ClientImpl::SetOnReceive(std::function<void(const std::string& data)> callback)
    {
        _onReceive = callback;
    }

    void ClientImpl::SetOnDisconnect(std::function<void()> callback)
    {
        _onDisconnect = callback;
    }

    void ClientImpl::Disconnect()
    {
        _running = false;

        if (_pollThread.joinable())
            _pollThread.join();
        if (_sendThread.joinable())
            _sendThread.join();

#ifdef _WIN32
        closesocket(_socket);
        WSACleanup();
#else
        close(_socket);
#endif
        if (_onDisconnect)
            _onDisconnect();
    }

    void ClientImpl::Connect(const std::string& ip, int port, const std::string& handshake)
    {
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
        _socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddress {};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &(serverAddress.sin_addr));

        if (connect(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            throw std::runtime_error("Failed to connect to server.");
        }

        _pollThread = std::thread(&ClientImpl::pollWorker, this);
        _sendThread = std::thread(&ClientImpl::sendWorker, this);

        Send(handshake);
    }

    // ===========================================================================
    // Receive
    // ===========================================================================

    void ClientImpl::pollWorker()
    {
        while (_running) {
            std::vector<myposix::Poll::aPollfd> pollfds;
            pollfds.emplace_back(myposix::Poll::aPollfd { .fd = _socket, .events = POLLIN, .revents = 0 });

            int timeout = _timeout;
            int ret = myposix::Poll::poll(pollfds, timeout);
            if (ret < 0) {
                CONSOLE::warn << "Client error: failed to poll the socket\n";
                continue;
            }

            if (pollfds[0].revents & POLLIN) {
                char buffer[1024] = { 0 };
                int valread = read(pollfds[0].fd, buffer, 1024);
                if (valread == 0) {
                    std::cout << "Server disconnected." << std::endl;
                    _onDisconnect();
                    return;
                }
                {
                    std::scoped_lock lock(_mutexIn);
                    _bufferIn.Write(buffer);
                }
            }
        }
    }

    void ClientImpl::Receive()
    {
        std::scoped_lock lock(_mutexIn);
        std::string data = "";
        while ((data = _bufferIn.ReadUntil("\r\n")) != "") {

            if (data.size() > 0 and _onReceive) {
                _onReceive(data);
            }
            data = "";
        }
    }

    // ===========================================================================
    // Send
    // ===========================================================================

    void ClientImpl::Send(const std::string& data)
    {
        std::scoped_lock lock(_mutexOut);
        std::cout << "in send function" << std::endl;
        _bufferOut.Write(data);
    }

    void ClientImpl::sendWorker()
    {
        while (_running) {
            std::vector<std::string> messages;

            {
                std::scoped_lock lock(_mutexOut);
                std::string tmp;
                while ((tmp = _bufferOut.ReadUntil("\r\n")) != "") {
                    messages.push_back(tmp);
                }
            }

            for (auto& message : messages) {
                std::cout << "socket sent : " << message << std::endl;
                send(_socket, message.c_str(), message.size(), 0);
            }
        }
    }

}