/*
** EPITECH PROJECT, 2023
** server
** File description:
** ClientUDP.hpp
*/

#pragma once
#include "CircularBuffer.hpp"
#include "Message.hpp"
#include "ThreadSafeQueue.hpp"
#include <atomic>
#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace serv {
    class ClientUDP;
    class IClientRequestHandler {
    public:
        virtual ~IClientRequestHandler() = default;
        virtual void HandleRequest(const bytes& data) = 0;
    };

    class AClientRequestHandler : public IClientRequestHandler {
    public:
        AClientRequestHandler(ClientUDP& client)
            : _client(client)
        {
        }

    protected:
        ClientUDP& _client;
    };

    class ClientUDP {
    public:
        ClientUDP();
        ~ClientUDP();

        void Send(const bytes& data);
        void Send(const Instruction& instruction);
        void SetServerAddress(const std::string& ip, int port);
        template <typename T>
        void SetRequestHandler()
        {
            _requestHandler = std::make_shared<T>(*this);
        }

        void SetRequestHandler(std::shared_ptr<IClientRequestHandler> handler);

        void Start();
        void Stop();

        /**
         * @brief Execute the request handler if it exists until the
         * request queue is empty.
         *
         */
        void CallHook();

    private:
        void sendWorker();
        void receiveWorker();
        std::thread _sendThread;
        std::thread _receiveThread;
        boost::asio::io_service _ioService;
        boost::asio::ip::udp::socket _socket;

        ThreadSafeQueue<bytes> _sendQueue;
        CircularBuffer _inBuffer;
        // ThreadSafeQueue<bytes> _inBuffer;
        std::shared_ptr<std::mutex>
            _mutex;
        std::atomic_bool _running;

        std::string _serverIp;
        int _serverPort;

        std::shared_ptr<IClientRequestHandler> _requestHandler;
    };
}