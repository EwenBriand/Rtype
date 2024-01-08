
/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.cpp
*/

#include "DistantPlayer.hpp"
#include "GameRtype.hpp"
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
    std::shared_ptr<eng::RType> rtype = std::dynamic_pointer_cast<eng::RType>(eng::Engine::GetEngine()->GetGame());
    rtype->GetSessionData() = eng::SessionData();
}

void DistantPlayer::HandleRequest(const serv::bytes& data)
{
    serv::Instruction instruction;
    try {
        instruction = serv::Instruction(data);
        if (_requestCallbacks.find(instruction.opcode) == _requestCallbacks.end()) {
            return;
        }
        auto handler = _requestCallbacks.at(instruction.opcode);
        if (handler == nullptr) {
            return;
        }
        (this->*_requestCallbacks.at(instruction.opcode))(instruction);
    } catch (const serv::MalformedInstructionException& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "\rMore informations: \n";
        std::cerr << "\r\traw size: " << data.size() << std::endl;
        std::cerr << "\r\tOpcode: " << instruction.opcode << std::endl;
        std::cerr << "\r\tExpects answer: " << instruction.expectsAnswer << std::endl;
        std::cerr << "\r\tData size: " << instruction.data.size() << std::endl;
        std::cerr << "\r\tData to string: " << data.toString() << std::endl;
        std::cerr << "\r\tData: ";
        for (auto& byte : data) {
            std::cerr << std::hex << (int)byte << " ";
        }
        std::cerr << std::endl;
        try {
            _server.Send(serv::Instruction(serv::E_INVALID_OPCODE, 0, serv::bytes(std::vector<int> { static_cast<int>(instruction.opcode) })), _endpoint);
        } catch (const std::exception& e) {
            std::cerr << "\rWhile handling invalid opcode: " << e.what() << std::endl;
        }
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
    for (auto it = Instances.begin(); it != Instances.end(); ++it) {
        if ((*it).get() == this) {
            Instances.erase(it);
            break;
        }
    }
    _server.Broadcast(serv::Instruction(eng::RType::I_RESET_CLIENT, 0, serv::bytes()));

    eng::Engine::GetEngine()->GetServer().ResetClients();
    //  todo switch Rtype state to lobby and clear all players
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

void DistantPlayer::handlePlayerMoves(serv::Instruction& instruction)
{
    if (instruction.data.size() < 3 * sizeof(int)) {
        std::cout << "\rinstruction size: " << instruction.data.size() << std::endl;
        throw serv::MalformedInstructionException("Player moves instruction malformed");
    }
    int id = 0;
    int x = 0;
    int y = 0;
    std::memcpy(&id, instruction.data.data(), sizeof(int));
    std::memcpy(&x, instruction.data.data() + sizeof(int), sizeof(int));
    std::memcpy(&y, instruction.data.data() + 2 * sizeof(int), sizeof(int));
    if (id != _playerId) {
        std::cerr << "\rplayer id: " << id << std::endl;
        throw serv::MalformedInstructionException("Player moves instruction malformed");
    }

    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        // todo anticheat goes here
        transform.x = x;
        transform.y = y;
    } catch (const std::exception& e) {
        return; // player died and will be removed
    }
    for (auto& player : Instances) {
        if (player->GetID() == _playerId) {
            continue;
        }
        player->Send(serv::Instruction(eng::RType::I_PLAYER_MOVES, 0, instruction.data));
    }
}

void DistantPlayer::handlePlayerShoots(serv::Instruction& instruction)
{
    eng::Engine::GetEngine()->GetServer().Log("Player shoots handler for player " + std::to_string(_playerId) + " called");
    if (instruction.data.size() != 3 * sizeof(int)) {
        throw serv::MalformedInstructionException("\rPlayer shoots instruction malformed: got " + std::to_string(instruction.data.size()) + " bytes, expected " + std::to_string(3 * sizeof(int)) + " bytes");
    }
    int id = 0;
    int x = 0;
    int y = 0;
    instruction.data.Deserialize(id, x, y);
    try {
        int laser = SYS.GetResourceManager().LoadPrefab("Laser");
        auto& transform = SYS.GetComponent<CoreTransform>(laser);
        transform.x = x;
        transform.y = y;
        for (auto& player : Instances) {
            if (player->GetID() == _playerId) {
                continue;
            }
            std::cout << "\rsending shoot instruction to player " << player->GetID() << std::endl;
            player->Send(serv::Instruction(eng::RType::I_PLAYER_SHOOTS, 0, serv::bytes(std::vector<int> { id, x, y })).ToBytes() + serv::SEPARATOR);
        }
    } catch (const std::exception& e) {
        CONSOLE::err << "\rFailed to send shoot instruction to clients." << std::endl;
    }
}

void DistantPlayer::handleDisconnect(serv::Instruction&)
{
    OnDisconnect();
}


void DistantPlayer::handleMessage(serv::Instruction& message)
{
    // get the server and broadcast the message to all players
    auto& server = eng::Engine::GetEngine()->GetServer();
    server.Broadcast(serv::Instruction(serv::I_MESSAGE, 0, message.data));
}