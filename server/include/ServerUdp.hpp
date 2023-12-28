/*
** EPITECH PROJECT, 2023
** server
** File description:
** ServerUdp.hpp
*/

#include "CircularBuffer.hpp"
#include "ClientUDP.hpp"
#include "Message.hpp"
#include "ThreadSafeQueue.hpp"
#include <atomic>
#include <boost/asio.hpp>
#include <chrono>
#include <fstream>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#pragma once

namespace serv {
    class ServerUDP;
    class IClient {
    public:
        virtual ~IClient() = default;
        virtual void HandleRequest(const bytes& data) = 0;
        virtual std::shared_ptr<IClient> Clone(boost::asio::ip::udp::endpoint endpoint) = 0;
        virtual void SetEndpoint(boost::asio::ip::udp::endpoint endpoint) = 0;
        virtual bool GetAnswerFlag() = 0;
        virtual void ResetAnswerFlag() = 0;
        virtual void OnDisconnect() = 0;
    };

    class AClient : public IClient {
    public:
        AClient(ServerUDP& server);

        void SetEndpoint(boost::asio::ip::udp::endpoint endpoint) override;
        void ResetAnswerFlag() override;
        bool GetAnswerFlag() override;

    protected:
        ServerUDP& _server;
        boost::asio::ip::udp::endpoint _endpoint;
        std::atomic_bool _answerFlag;
    };

    static const bytes SEPARATOR("msgdone");
    static const std::size_t BUFF_SIZE = 2048;
    class ServerUDP;
    /**
     * @brief Each client has a bucket, which contains all the information
     * about the client as well as the functions to handle its requests.
     *
     */
    class ClientBucketUDP {
    public:
        ClientBucketUDP(boost::asio::ip::udp::endpoint endpoint);
        ~ClientBucketUDP();

        /**
         * @brief Writes data in the buffer in a thread safe manner.
         *
         * @param data
         */
        void Write(const bytes& data);

        /**
         * @brief Reads the buffer until SEPARATOR in a thread safe manner.
         *
         * @return std::string
         */
        bytes Read();

        /**
         * @brief returns the endpoint of the client
         *
         * @return asio::ip::udp::endpoint
         */
        boost::asio::ip::udp::endpoint GetEndpoint() const;

        /**
         * @brief Set the handle Request object
         *
         * @param handleRequest
         */
        template <typename T>
        void SetHandleRequest()
        {
            _clientHandler = std::make_shared<T>();
        }

        void SetHandleRequest(std::shared_ptr<IClient> clientHandler);

        /**
         * @brief Handles the requests of the client. (synchronous)
         *
         * @param server
         */
        void HandleRequest(ServerUDP& server);

        /**
         * @brief Sets the last time the client sent a request to now.
         *
         */
        void UpdateLastRequestTime();

        /**
         * @brief Returns how long ago in seconds was the last request time (see UpdateLastRequestTime)
         *
         * @return std::chrono::time_point<std::chrono::system_clock>
         */
        float GetLastRequestTime();

        /**
         * @brief Calls the OnDisconnect method of the client handler.
         *
         */
        void OnDisconnect();

    private:
        boost::asio::ip::udp::endpoint _endpoint;
        std::function<void()> _requestHook;
        std::shared_ptr<IClient> _clientHandler;
        std::shared_ptr<std::mutex> _mutex;
        CircularBuffer _buffer;
        std::chrono::time_point<std::chrono::system_clock> _lastRequestTime;
        std::shared_ptr<std::mutex> _lastRequestTimeMutex;
    };

    class ServerUDP {
    public:
        static constexpr std::size_t BUFF_SIZE = 1024;
        ServerUDP(unsigned short port);
        ~ServerUDP();

        template <typename T>
        void SetHandleRequest()
        {
            _clientHandlerCopyBase = std::make_shared<T>(*this, false);
        }
        /**
         * @brief Receives requests from clients and adds them to the
         * _requestQueue.
         *
         */
        void HandleRequest(
            const boost::system::error_code& error, std::size_t bytesTransferred);

        /**
         * @brief Buffers a message to be sent asynchronously.
         *
         */
        void Send(const Message& message);

        /**
         * @brief Sends an instruction.
         *
         */
        void Send(const Instruction& instruction, const boost::asio::ip::udp::endpoint& endpoint);

        /**
         * @brief Start the server.
         *
         */
        void Start();

        /**
         * @brief Sends the same message to all clients
         *
         */
        void Broadcast(const bytes& message);

        /**
         * @brief Broadcasts an instruction
         *
         */
        void Broadcast(const Instruction& instruction);

        /**
         * @brief Calls the HandleRequest function of all clients. Until there are no more
         * requests in the queue.
         *
         */
        void CallHooks();

        /**
         * @brief Returns the string representation of bytes.
         *
         */
        static std::string bytesToString(const bytes& bytes);

        /**
         * @brief Returns the bytes corresponding to the string.
         *
         */
        static bytes stringToBytes(const std::string& string);

        /**
         * @brief Logs an entry in the log file.
         *
         */
        void Log(const std::string& entry);

    private:
        /**
         * @brief Receives strings and tries assigns them to client buckets asynchronously.
         *
         */
        void receiveWorker();

        /**
         * @brief Continuously sends messages from the _sendQueue asynchronously.
         *
         */
        void sendWorker();

        /**
         * @brief checks if any client has disconnected, and if so, removes it from the list of clients.
         *
         */
        void checkForDisconnections();

        boost::asio::io_service _ioService;
        boost::asio::ip::udp::socket _socket;
        boost::asio::ip::udp::endpoint _endpoint;
        boost::asio::ip::udp::endpoint _remoteEndpoint;

        std::string endpointToString(boost::asio::ip::udp::endpoint endpoint);

        std::array<char, BUFF_SIZE> _buffer;

        std::map<std::string, std::shared_ptr<ClientBucketUDP>> _clients;
        std::shared_ptr<std::mutex> _clientsMutex;

        std::thread _receiveThread;

        std::thread _sendThread;
        ThreadSafeQueue<Message> _sendQueue;

        std::thread _checkForDisconnectionsThread;

        std::atomic_bool _running = false;

        std::shared_ptr<IClient> _clientHandlerCopyBase;

        std::ofstream _logFile;
        std::shared_ptr<std::mutex> _logMutex;
    };
}