/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.hpp
*/

#pragma once

#include "ClientUDP.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "NetworkExceptions.hpp"
#include "PlayerFromServerController.hpp"
#include <atomic>
#include <map>
#include <memory>

namespace rtype {
    class RTypeDistantServer : public serv::AClientRequestHandler {

    public:
        static RTypeDistantServer* Instance;

        static RTypeDistantServer* GetInstance();

        RTypeDistantServer(serv::ClientUDP& client);
        ~RTypeDistantServer() override;

        void SetAsInstance();
        void SetEngine(eng::Engine* eng);
        void HandleRequest(const serv::bytes& data) override;

        /**
         * @brief Tries to connect to the server.
         *
         */
        void TryConnect();

        /**
         * @brief Returns true if the distant server is connected.
         *
         */
        bool IsConnected() const;

        /**
         * @brief Returns true if all players have loaded the scene.
         * This will be triggered by the server sending the I_START_GAME
         * instruction.
         *
         */
        bool ShouldStartGame();

        /**
         * @brief Returns true if the scene has been loaded.
         *
         * @return true
         * @return false
         */
        bool SceneIsReady();

        /**
         * @brief Instantiates the scene if it is ready.
         *
         */
        void InstantiateScene();

    private:
        void handleConnectOk(serv::Instruction& instruction);
        void handleServerFull(serv::Instruction& instruction);
        void handleLoadScene(serv::Instruction& instruction);
        void handleStartGame(serv::Instruction& instruction);
        void handleMessage(serv::Instruction& instruction);
        void handleAssignPlayerID(serv::Instruction& instruction);
        void handlePlayerSpawn(serv::Instruction& instruction);

        /**
         * @brief Sends the I_AM_ALIVE instruction to the server.
         *
         */
        void pingServerForAlive();
        std::thread _pingThread;

        bool _isConnected = false;
        bool _startGame = false;
        std::string _currSceneName = "";

        eng::Engine* _engine = nullptr;

        int _playerId;

        std::map<int, std::shared_ptr<PlayerFromServerController>> _players;

        std::map<int, void (RTypeDistantServer::*)(serv::Instruction&)>
            _requestHandlers = {
                { serv::I_CONNECT_OK, &RTypeDistantServer::handleConnectOk },
                { serv::E_SERVER_FULL, &RTypeDistantServer::handleServerFull },
                { serv::I_LOAD_SCENE, &RTypeDistantServer::handleLoadScene },
                { serv::I_START_GAME, &RTypeDistantServer::handleStartGame },
                { serv::I_MESSAGE, &RTypeDistantServer::handleMessage },

                { eng::RType::I_PLAYER_ID, &RTypeDistantServer::handleAssignPlayerID },
                { eng::RType::I_PLAYER_SPAWN, &RTypeDistantServer::handlePlayerSpawn },
            };
    };
} // namespace rtype
