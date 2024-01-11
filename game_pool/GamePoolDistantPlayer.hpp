/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** GamePoolDistantPlayer.hpp
*/

#pragma once

#include "GamePool.hpp"
#include "IController.hpp"
#include "NetworkExceptions.hpp"

namespace pool {
    class GPDistantPlayer : public serv::AClient {
        public:
            static constexpr int POOL_NB_PLAYERS = 2;
            static std::vector<std::shared_ptr<GPDistantPlayer>> Instances; 

            using requestHandler = void (GPDistantPlayer::*)(int expectedReturnCode, const serv::bytes& data);
            static const std::map<int, requestHandler> RequestHandlers;

            GPDistantPlayer(serv::ServerUDP& server, bool send = true);
            ~GPDistantPlayer() override;

            void OnDisconnect() override; 
            std::shared_ptr<IClient> Clone(boost::asio::ip::udp::endpoint endpoint) override;

            inline void Send(const serv::Instruction &instruction) {
                _server.Send(instruction, _endpoint);
            }

            void HandleRequest(const serv::bytes& data) override;
            std::shared_ptr<serv::IClient> Clone(boost::asio::ip::udp::endpoint) override;
        private:
            bool _isConnected = false;
            bool _startGame = false;
    };
}