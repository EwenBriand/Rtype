
/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.cpp
*/

#include "DistantPlayer.hpp"
#include "LobbyCoroutine.hpp"
#include "NetworkExceptions.hpp"
#include "ServerUdp.hpp"
#include <iostream>

std::vector<std::shared_ptr<DistantPlayer>> DistantPlayer::Instances;
const std::map<int, DistantPlayer::requestHandler> DistantPlayer::RequestHandlers = {};

DistantPlayer::DistantPlayer(serv::ServerUDP& server, bool send)
    : serv::AClient(server)
{
    if (Instances.size() >= rtype::RTYPE_NB_PLAYERS) {
        throw serv::NetworkException(_server, serv::E_SERVER_FULL, "Server is full", _endpoint);
    }
}

DistantPlayer::~DistantPlayer()
{
}

serv::bytes DistantPlayer::HandleRequest(const serv::bytes& data)
{
    return serv::bytes("OK") + serv::SEPARATOR;
}

std::shared_ptr<serv::IClient> DistantPlayer::Clone(boost::asio::ip::udp::endpoint endpoint)
{
    if (Instances.size() >= rtype::RTYPE_NB_PLAYERS) {
        throw serv::NetworkException(_server, serv::E_SERVER_FULL, "Server is full", endpoint);
    }

    auto copy = std::make_shared<DistantPlayer>(_server);

    if (copy == nullptr) {
        throw serv::NetworkException(_server, serv::E_SERVER_INTERNAL_ERROR, "Could not create new player", endpoint);
    }

    Instances.push_back(copy);
    copy->SetEndpoint(_endpoint);
    _server.Send(serv::Instruction(serv::I_CONNECT_OK, 0, serv::bytes()), endpoint);
    return copy;
}

void DistantPlayer::SendClientLoadScene(const std::string& sceneName)
{
    auto instruction = serv::Instruction(
        serv::I_LOAD_SCENE,
        serv::I_LOAD_SCENE % 255,
        serv::bytes(sceneName));
    _server.Send(instruction, _endpoint);
}

void DistantPlayer::SendClientStartGame()
{
    auto instruction = serv::Instruction(
        serv::I_START_GAME,
        0,
        serv::bytes());
    _server.Send(instruction, _endpoint);
}

// =======================================================================
// REQUEST HANDLING METHODS
// =======================================================================

void DistantPlayer::handleOK(serv::Instruction&)
{
    std::cout << "DistantPlayer::handleOK" << std::endl;
    _answerFlag = true;
}