/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** GamePoolDistantPlayer.cpp
*/

#include "GamePoolDistantPlayer.hpp"

namespace pool {
    std::vector<std::shared_ptr<GamePoolDistantPlayer>> GamePoolDistantPlayer::Instances;

    GPDistantPlayer::GPDistantPlayer(serv::ServerUDP& server, bool send)
        : serv::AClient(server)
    {
        if (Instances.size() >= pool::POOL_NB_PLAYERS) {
            throw serv::NetworkException(_server, serv::E_SERVER_FULL, "Lobby is full", _endpoint);
        }
    }

    GPDistantPlayer::~GPDistantPlayer() {

    }

    void GPDistantPlayer::OnDisconnect() {
        std::cout << "\rClient disconnected" << std::endl;
        for (auto it = Instances.begin(); it != Instances.end(); ++it) {
            if ((*it).get() == this) {
                Instances.erase(it);
                break;
            }
        }
        // todo make sure that the this below works
        // _server.Broadcast(serv::Instruction(eng::GamePoolGame::I_RESET_CLIENT, 0, serv::bytes()));
    }

    std::shared_ptr<serv::IClient> GPDistantPlayer::Clone(boost::asio::ip::udp::endpoint endpoint) {
        if (Instances.size() >= 2) {
            throw serv::NetworkException(_server, serv::E_SERVER_FULL, "Lobby is full", endpoint);
        }

        auto copy = std::make_shared<GPDistantPlayer>(_server);

        if (copy == nullptr) {
            throw serv::NetworkException(_server, serv::E_SERVER_INTERNAL_ERROR, "Could not create client");
        }
        Instances.push_back(copy); 
        copy->SetEndpoint(_endpoint);
        _server.Send(server::Instruction(serv::I_CONNECT_OK, 0, serv::bytes()), endpoint);
        return copy;
    }

    bool GPDistantPlayer::ShouldStartGame() {
        return _startGame;
    }
    
    void GPDistantPlayer::handleStartGame(serv::Instruction& instruction)
    {
        _startGame = true;
    }
}