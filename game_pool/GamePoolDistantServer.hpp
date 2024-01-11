/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** GamePoolDistantServer.hpp
*/

#pragma once
#include "Engine.hpp"
#include "NetworkExceptions.hpp"
#include "ClientUDP.hpp"
#include "GamePool.hpp"
#include <map>
#include <memory>

namespace pool {
    class PoolDistantServer : public serv::AClientRequestHandler {
        public:
            PoolDistantServer(serv::ClientUDP& client);

            void HandleRequest(const serv::bytes& data) override;
        
        private:
            std::map<std::string, void (eng::GamePool::*)(serv::Instruction)> _requestHandlers = {

            };
    }
}