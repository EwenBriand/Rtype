/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.hpp
*/

#pragma once
#include "NetworkExceptions.hpp"
#include "ServerUdp.hpp"
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <vector>

class DistantPlayer : public serv::AClient {
public:
    static std::vector<std::shared_ptr<DistantPlayer>> Instances;

    using requestHandler = void (DistantPlayer::*)(int expectedReturnCode, const serv::bytes& data);
    static const std::map<int, requestHandler> RequestHandlers;

    DistantPlayer(serv::ServerUDP& server, bool send = true);
    ~DistantPlayer() override;

    void OnDisconnect() override;

    /**
     * @brief Sends a request to the client to load the scene specified.
     *
     * @param sceneName
     */
    void SendClientLoadScene(const std::string& sceneName);

    /**
     * @brief Sends the signal to the client that he can start playing the game
     * because all the other players are ready.
     *
     */
    void SendClientStartGame();

    serv::bytes HandleRequest(const serv::bytes& data) override;
    std::shared_ptr<serv::IClient> Clone(boost::asio::ip::udp::endpoint endpoint) override;

private:
    void handleOK(serv::Instruction&);

    std::map<int, void (DistantPlayer::*)(serv::Instruction&)> _requestCallbacks = {
        { serv::I_OK, &DistantPlayer::handleOK }
    };
};