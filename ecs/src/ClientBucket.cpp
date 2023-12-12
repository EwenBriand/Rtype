/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ClientBucket.cpp
*/

#include "Server.hpp"

namespace serv {
    void ServerImpl::ClientBucket::SetClient(std::shared_ptr<IClient> client)
    {
        _client = client;
    }

    void ServerImpl::ClientBucket::Send(const std::string &data)
    {
        *this << Message(data, 0);
    }

    void ServerImpl::ClientBucket::Receive(const std::string &data)
    {
        _buffer.Write(data);
        std::string message = _buffer.ReadUntil("\r\n");
        if (_client == nullptr) {
            std::cout << "client is null!" << std::endl;
            return;
        }
        std::cout << '[' << message << ']' << std::endl;
        while (!message.empty()) {
                message = message.substr(0, message.size() - 2);
                _tasks->Push([this, message]() {
                    try {
                        auto answer = _client->HandleRequest(message);
                        *this << answer;
                    } catch (const HandleRequestException &e) {
                        *this << e.GetMessage();
                        std::cout << e.what() << std::endl;
                    }
                });
            message = _buffer.ReadUntil("\r\n");
        }
    }

    ServerImpl::ClientBucket &ServerImpl::ClientBucket::operator<<(const Message &message)
    {
        ServerImpl::Get()->BufferMessage(std::to_string(message.GetCode()) + " " + message.GetData() + "\r\n", _socket);
        return *this;
    }

    std::shared_ptr<ServerImpl::IClient> ServerImpl::ClientBucket::GetClient() const
    {
        return _client;
    }

#ifdef _WIN32
    long long ServerImpl::ClientBucket::GetSocket() const
#else
    int ServerImpl::ClientBucket::GetSocket() const
#endif
    {
        return _socket;
    }

    ServerImpl::ClientBucket::ClientBucket(int socket) :
        _buffer(2048), _socket(socket), _tasks(std::make_shared<td::ThreadedQueue<50>>())
    {
    }

    ServerImpl::ClientBucket::~ClientBucket()
    {
    }

}
