/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** NetworkExceptions.hpp
*/

#pragma once
#include "ServerUdp.hpp"
#include <boost/asio.hpp>
#include <stdexcept>

namespace serv {
    class NetworkException : public std::exception {
    public:
        NetworkException(ServerUDP& server, int errorCode, const std::string& message, boost::asio::ip::udp::endpoint endpoint)
            : _message(message)
        {
            try {
                server.Send(serv::Instruction(errorCode, errorCode, serv::bytes(message)), endpoint);
            } catch (const std::exception& e) {
                std::cerr << "Could not send error message: " << e.what() << std::endl;
            }
        }

        const char* what() const noexcept override
        {
            return _message.c_str();
        }

    private:
        std::string _message;
    };

    static int I_OK = 0;
    static int I_CONNECT = 1001;
    static int I_CONNECT_OK = 1002;
    static int I_LOAD_SCENE = 1003;
    static int I_SCENE_LOADED = 1004;
    static int I_START_GAME = 1005;

    static int E_SERVER_FULL = 300;
    static int E_INVALID_OPCODE = 301;
    static int E_SERVER_INTERNAL_ERROR = 302;

} // namespace serv
