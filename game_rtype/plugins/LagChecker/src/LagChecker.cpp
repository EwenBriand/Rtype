/*
** EPITECH PROJECT, 2023
** safe
** File description:
** LagChecker.cpp
*/

#include "LagChecker.hpp"
#include <iostream>

extern "C" {
void updateStats()
{
    static metrics::LagChecker lagChecker;
    lagChecker.Tik();
}
}

namespace metrics {
    LagChecker::LagChecker()
        : _logFile(".lag.log")
        , _mutex(std::make_shared<std::mutex>())
    {
        init();
    }

    void LagChecker::Tik()
    {
        sendPingToClients();
        logAverageLag();
    }

    std::string LagChecker::endpointToString(boost::asio::ip::udp::endpoint endpoint)
    {
        return endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
    }

    void LagChecker::init()
    {
        auto engine = eng::Engine::GetEngine();
        if (engine->IsClient()) {
            engine->GetClient().Intercept(I_LAG_PROBE, [this](const serv::Instruction& instruction) {
                clientSendResponseToServer(instruction);
            });
        } else {
            engine->GetServer().Intercept(I_LAG_PROBE, [this](const serv::Instruction& instruction, boost::asio::ip::udp::endpoint& endpoint) {
                serverManageResponseFromClient(instruction, endpoint);
            });
        }
    }

    void LagChecker::clientSendResponseToServer(const serv::Instruction& instruction)
    {
        eng::Engine::GetEngine()->GetClient().Send(instruction);
    }

    void LagChecker::serverManageResponseFromClient(const serv::Instruction& instruction, boost::asio::ip::udp::endpoint& endpoint)
    {
        std::string clientKey = endpointToString(endpoint);
        if (_clients.find(clientKey) == _clients.end()) {
            std::scoped_lock lock(*_mutex);
            _clients[clientKey] = ClientInfo();
            return;
        }
        try {
            int pingId = instruction.expectsAnswer;
            std::scoped_lock lock(*_mutex);

            _clients[clientKey]._pings[pingId]._receivedTime = std::chrono::high_resolution_clock::now();
            _clients[clientKey]._resolvedPings.push_back(_clients[clientKey]._pings[pingId]);
            _clients[clientKey]._pings.erase(pingId);
            if (_clients[clientKey]._resolvedPings.size() > 10) {
                _clients[clientKey]._resolvedPings.erase(_clients[clientKey]._resolvedPings.begin());
            }
            float averageLag = 0;
            for (auto& ping : _clients[clientKey]._resolvedPings) {
                averageLag += std::chrono::duration_cast<std::chrono::milliseconds>(ping._receivedTime - ping._sentTime).count();
            }
            if (_clients[clientKey]._resolvedPings.size() == 0)
                return;
            averageLag /= _clients[clientKey]._resolvedPings.size();
            _clients[clientKey].averageLag = averageLag;
        } catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void LagChecker::sendPingToClients()
    {
        if (not eng::Engine::GetEngine()->IsServer())
            return;
        eng::Engine::GetEngine()->GetServer().Broadcast(serv::Instruction(I_LAG_PROBE, _instrID, serv::bytes()));
        for (auto& client : _clients) {
            std::scoped_lock lock(*_mutex);
            client.second._pings[_instrID]._sentTime = std::chrono::high_resolution_clock::now();
        }
        _instrID++;
    }

    void LagChecker::logAverageLag()
    {
        if (not eng::Engine::GetEngine()->IsServer())
            return;
        std::string log = "";
        std::scoped_lock lock(*_mutex);
        for (auto& client : _clients) {
            log += "(" + client.first + ": " + std::to_string(client.second.averageLag) + " ms) ";
        }
        _logFile << log << std::endl;
    }
} // namespace metrics
