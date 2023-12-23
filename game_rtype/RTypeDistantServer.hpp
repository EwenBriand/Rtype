/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.hpp
*/

#pragma once

#include "ClientUDP.hpp"
#include "NetworkExceptions.hpp"
#include <map>
#include <memory>

namespace rtype {
    class RTypeDistantServer : public serv::AClientRequestHandler {

    public:
        static RTypeDistantServer* Instance;

        static RTypeDistantServer& GetInstance();

        RTypeDistantServer(serv::ClientUDP& client);
        ~RTypeDistantServer() override = default;

        void SetAsInstance();
        void HandleRequest(const serv::bytes& data) override;

        /**
         * @brief Tries to connect to the server.
         *
         */
        void TryConnect();

        /**
         * @brief Returns true if the distant server is connected.
         *
         */
        bool IsConnected() const;

    private:
        void handleConnectOk(serv::Instruction& instruction);
        void handleServerFull(serv::Instruction& instruction);

        bool _isConnected = false;

        std::map<int, void (RTypeDistantServer::*)(serv::Instruction&)> _requestHandlers = {
            { serv::I_CONNECT_OK, &RTypeDistantServer::handleConnectOk },
            { serv::E_SERVER_FULL, &RTypeDistantServer::handleServerFull }
        };
    };
} // namespace rtype
