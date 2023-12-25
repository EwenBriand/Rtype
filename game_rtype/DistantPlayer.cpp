
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

void DistantPlayer::HandleRequest(const serv::bytes& data)
{
    try {
        auto instruction = serv::Instruction(data);

        if (_requestCallbacks.find(instruction.opcode) == _requestCallbacks.end()) {
            throw serv::MalformedInstructionException("Unknown instruction received from client: " + std::to_string(instruction.opcode));
        }
        auto handler = _requestCallbacks.at(instruction.opcode);
        if (handler == nullptr) {
            return;
        }
        (this->*_requestCallbacks.at(instruction.opcode))(instruction);
    } catch (const serv::MalformedInstructionException& e) {
        std::cerr << e.what() << std::endl;
        _server.Send(serv::Instruction(serv::E_INVALID_OPCODE, 0, serv::bytes()), _endpoint);
    }
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

void DistantPlayer::OnDisconnect()
{
    std::cout << "\rClient disconnected" << std::endl;
    // todo possess player with ai instead of distant player
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

void DistantPlayer::SetID(int id)
{
    _playerId = id;
}

int DistantPlayer::GetID() const
{
    return _playerId;
}

// =======================================================================
// REQUEST HANDLING METHODS
// =======================================================================

void DistantPlayer::handleOK(serv::Instruction&)
{
    std::cout << "DistantPlayer::handleOK" << std::endl;
    _answerFlag = true;
}

// =======================================================================
// ICONTROLLER METHODS
// =======================================================================

std::vector<std::string>& DistantPlayer::GetDirectives()
{
    return _directives;
}

void DistantPlayer::PollDirectives()
{
    _directives.clear();
}

void DistantPlayer::UpdatePipeline()
{
}

void DistantPlayer::SetEntity(int entityID)
{
    _entityID = entityID;
}