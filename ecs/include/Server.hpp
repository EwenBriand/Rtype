/*
** EPITECH PROJECT, 2023
** ecs
** File description:
** Server.hpp
*/

#pragma once


#include <memory>
#include <map>
#include <string>
#include <optional>
#include <ctime>
#include <queue>
#include <tuple>
#include <vector>
#include <functional>
#include <poll.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include "Component.hpp"
#include "ThreadPool.hpp"

#define registerHandshake(type) RegisterHandshake<type>(#type)


namespace serv {
    /**
     * @brief This class implements a circular buffer. The user can Read the whole
     * buffer at once or stop at a given delimiter.
     *
     */
    class CircularBuffer {
        public:
            CircularBuffer(int size);
            ~CircularBuffer();

            /**
             * @brief Writes some data in the buffer.
             *
             * @param data
             */
            void Write(const std::string &data);

            /**
             * @brief Returns the data in the buffer until the given delimiter,
             * or the empty string if the delimiter is not found.
             *
             * @param delimiter
             * @return std::string
             */
            std::string ReadUntil(const std::string &delimiter);

            /**
             * @brief Returns the whole buffer.
             *
             * @return std::string
             */
            std::string Read();

            /**
             * @brief Returns the size of the buffer.
             *
             * @return int
             */
            int GetSize() const;

            /**
             * @brief Returns true if the buffer is empty, false otherwise.
             *
             * @return true
             * @return false
             */
            bool IsEmpty() const;

            /**
             * @brief Returns true if the buffer is full, false otherwise.
             *
             * @return true
             * @return false
             */
            bool IsFull() const;
        private:
            int _size;
            int _read;
            int _write;
            char *_buffer;
    };


    /**
     * @brief This class implements a server capable of
     * handling mutliple clients at once.
     * Each client is represented by a ClientBucket object, which can
     * contain a different implementation of the IClient interface.
     *
     * Every message received by the server from a socket will be transmitted
     * to the corresponding ClientBucket object.
     *
     * A ClientBucket is automatically created when a client connects to the
     * server. It will remain empty until the client performs the handshake with
     * the server.
     *
     * The handshake should go as follows:
     * 1. Client connects to the server.
     * 2. Server sends message: "Connection OK, waiting for handshake."
     * 3. Client sends message: "Handshake: <client_type>"
     * 4. Server sends message: "Handshake OK."
     *
     * In case of an error, the server will send the message
     * "Error: <error_message>"
     *
     */
    class ServerImpl {
        public:

            class Message {
                public:
                    static const Message WaitingForHandshake;
                    static const Message HandshakeOK;

                    Message(const std::string &data, int code = 200);
                    ~Message() = default;

                    std::string GetData() const;
                    std::time_t GetTime() const;
                    int GetCode() const;

                private:
                    std::string _data;
                    std::time_t _time;
                    int _code;
            };

            /**
             * @brief This IClient interface is used to represent a distant client
             * and store its associated data.
             *
             */
            class IClient {
                public:
                    virtual ~IClient() = default;
                    virtual Message HandleRequest(const std::string &data) = 0;
            };

            class HandleRequestException : public std::exception {
                public:
                    HandleRequestException(const std::string &message, int code);
                    ~HandleRequestException() = default;

                    const char *what() const noexcept override;
                    const Message GetMessage() const;

                private:
                    int _code;
                    std::string _message;
            };

            class ClientBucket {
                public:
                    ClientBucket(int port);
                    ~ClientBucket();

                    void SetClient(std::shared_ptr<IClient> client);
                    void Send(const std::string &data);
                    void Receive(const std::string &data);
                    std::shared_ptr<IClient> GetClient() const;
                    int GetSocket() const;

                    ClientBucket &operator<<(const Message &message);

                template <typename T>
                static std::shared_ptr<IClient> GenerateClient() {
                    return std::static_pointer_cast<IClient>(std::make_shared<T>());
                }

                private:
                    CircularBuffer _buffer;
                    std::shared_ptr<IClient> _client;
                    std::shared_ptr<td::ThreadedQueue<50>> _tasks;
                    int _socket;
            };

            static ServerImpl *Get();
            ServerImpl(const ServerImpl &cpy) = delete;
            ServerImpl &operator=(const ServerImpl &src) = delete;
            ServerImpl();
            ~ServerImpl();

            void SetPort(int port);
            void SetMaxClients(int maxClients);

            void BufferMessage(const std::string &data, int socket);

            /**
             * @brief This method should be called in a loop to poll the server.
             *
             */
            void Poll();

            /**
             * @brief This method creates a new client bucket and adds it to the
             * list of clients.
             * It also sends the handshake message to the client.
             *
             */
            void AcceptNewClient();

            template <typename T>
            void RegisterHandshake(const std::string &name) {
                std::cout << "registered handshake [" << name << "]" << std::endl;
                _handshakes[name] = ClientBucket::GenerateClient<T>;
            }

            /**
             * @brief This method initializes the server. It has to be called before
             * being able to receive clients.
             *
             */
            void Start();

        private:
            void SendBufferedMessages();

            /**
             * @brief This method checks if the given client has performed the handshake.
             * If it has, it will return true, otherwise it will return false.
             * If the handshake is performed, it will also set the client's type.
             *
             * @param client
             * @param buffer
             * @return true
             * @return false
             */
            bool checkForHandshake(ClientBucket &client, const std::string &buffer);


            static std::unique_ptr<ServerImpl> _instance;
            std::vector<ClientBucket> _clients;

            int _port;
            int _maxClients = -1;
            int _socket;
            struct sockaddr_in _address;
            socklen_t _addrlen;

            fd_set _readfds;
            fd_set _writefds;
            fd_set _exceptfds;

            std::map<std::string, std::function<std::shared_ptr<IClient>()>> _handshakes;
            std::vector<pollfd> _pollfds;
            std::map<int, std::queue<std::string>> _bufferedMessages;
    };

    /**
     * @brief This class wraps the ServerImpl class to make it compatible with the ECS.
     *
     */
    class Server : public ecs::Component<Server> {
    public:
        // Component methods
        void Update(int entityID);
        void OnLoad();
        void Start();
        void Load(const std::string &path);
        void Save(const std::string &path);
        void OnAddComponent(int entityID);
        std::map<std::string, metadata_t> GetMetadata();
        std::string GetClassName() const;

        private:
    };
}
