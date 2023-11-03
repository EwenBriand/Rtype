/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Message.cpp
*/

#include "Server.hpp"

namespace serv {
    const ServerImpl::Message ServerImpl::Message::WaitingForHandshake = Message("Connection OK, waiting for handshake.", 0);
    const ServerImpl::Message ServerImpl::Message::HandshakeOK = Message("Handshake OK", 0);

    ServerImpl::Message::Message(const std::string &data, int code)
    {
        _data = data;
        _time = std::time(nullptr);
        _code = code;
    }

    int ServerImpl::Message::GetCode() const
    {
        return _code;
    }

    std::string ServerImpl::Message::GetData() const
    {
        return _data;
    }

    std::time_t ServerImpl::Message::GetTime() const
    {
        return _time;
    }

    const ServerImpl::Message ServerImpl::HandleRequestException::GetMessage() const
    {
        return Message("Request Error: " + std::to_string(_code) + " " + _message);
    }
}
