/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Client.hpp
*/

#pragma once

#include "Server.hpp"
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace serv {
    /**
     * @brief This class encapsulates a client connection to a distant server.
     * The client can send and receive data from the server, and can be assigned a
     * callback that will be called when data is received.
     *
     */
    class ClientImpl {
    public:
        ClientImpl(int timeout = 1000);
        ~ClientImpl();

        void Send(const std::string& data);
        void SetOnReceive(std::function<void(const std::string& data)> callback);
        void SetOnDisconnect(std::function<void()> callback);
        void Disconnect();
        void Connect(const std::string& ip, int port, const std::string& handshake);
        void Receive();

    private:
        void pollWorker();
        void sendWorker();
        std::function<void(const std::string& data)> _onReceive;
        std::function<void()> _onDisconnect;
        CircularBuffer _bufferOut;
        CircularBuffer _bufferIn;
        int _socket;
        std::mutex _mutexOut;
        std::mutex _mutexIn;

        std::thread _pollThread;
        std::thread _sendThread;
        std::atomic_bool _running = true;

        int _timeout;
    };
} // namespace serv
