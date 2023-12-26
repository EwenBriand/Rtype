/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.hpp
*/

#pragma once
#include "GameRtype.hpp"
#include "IController.hpp"
#include "Message.hpp"
#include "NetworkExceptions.hpp"
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <vector>

class DistantPlayer : public serv::AClient, public ecs::IController {
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

    /**
     * @brief Assigns a unique id to the player.
     *
     * @param id
     */
    void SetID(int id);

    /**
     * @brief Returns the player's id.
     *
     * @return int
     */
    int GetID() const;

    /**
     * @brief Sends a message to the client associated with this bucket.
     *
     * @param data
     * @return serv::bytes
     */
    inline void Send(const serv::Instruction& instruction)
    {
        _server.Send(instruction, _endpoint);
    }

    std::vector<std::string>& GetDirectives() override;
    void PollDirectives() override;
    void UpdatePipeline() override;
    void SetEntity(int entityID) override;

    void HandleRequest(const serv::bytes& data) override;
    std::shared_ptr<serv::IClient> Clone(boost::asio::ip::udp::endpoint endpoint) override;

private:
    void handleOK(serv::Instruction&);
    void handlePlayerMoves(serv::Instruction&);

    int _playerId;
    int _entityID;
    std::vector<std::string> _directives;

    std::map<int, void (DistantPlayer::*)(serv::Instruction&)> _requestCallbacks = {
        { serv::I_OK, &DistantPlayer::handleOK },
        { eng::RType::I_PLAYER_MOVES, &DistantPlayer::handlePlayerMoves },
        { serv::I_CONNECT, nullptr },
        { serv::I_AM_ALIVE, nullptr },
    };
};
