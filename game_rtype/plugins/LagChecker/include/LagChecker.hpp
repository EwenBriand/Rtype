/*
** EPITECH PROJECT, 2023
** safe
** File description:
** LagChecker.hpp
*/

#pragma once
#include "ClientUDP.hpp"
#include "Engine.hpp"
#include "ServerUdp.hpp"
#include <chrono>
#include <fstream>
#include <memory>
#include <mutex>

namespace metrics {
    class LagChecker {
    public:
        static constexpr int I_LAG_PROBE = 5641973;
        struct PingInfo {
            std::chrono::high_resolution_clock::time_point _sentTime;
            std::chrono::high_resolution_clock::time_point _receivedTime;
        };

        struct ClientInfo {
            std::map<int, PingInfo> _pings;
            std::vector<PingInfo> _resolvedPings;
            float averageLag = 0;
        };

        LagChecker();

        void Tik();

    private:
        void init();
        void clientSendResponseToServer(const serv::Instruction& instruction);
        void serverManageResponseFromClient(const serv::Instruction& instruction, boost::asio::ip::udp::endpoint& endpoint);
        void sendPingToClients();
        void logAverageLag();

        std::string endpointToString(boost::asio::ip::udp::endpoint endpoint);
        std::map<std::string, ClientInfo> _clients;
        std::shared_ptr<std::mutex> _mutex;
        unsigned char _instrID = 0;
        std::ofstream _logFile;
    };
}