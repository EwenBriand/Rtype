/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.hpp
*/

#pragma once

#include "ClientUDP.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "NetworkExceptions.hpp"
#include "Observer.hpp"
#include "PlayerFromServerController.hpp"
#include <atomic>
#include <map>
#include <memory>

namespace rtype {
    class RTypeDistantServer : public serv::AClientRequestHandler {

    public:
        static const std::vector<std::string> PlayerPrefabs;
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

        /**
         * @brief Resets all data, ready to start a new game.
         *
         */
        void Reset();

        /**
         * @brief Returns true if the game should be reset.
         *
         */
        bool ShouldReset();

        /**
         * @brief Reset's the reset flag.
         *
         */
        void ResetReset();

        /**
         * @brief Set kill count.
         *
         * @param kill_count
         */
        void SetKillCount(int kill_count) noexcept;

        /**
         * @brief Returns the kill count.
         *
         * @return int
         */
        int GetKillCount() const noexcept;

    private:
        void handleConnectOk(serv::Instruction& instruction);
        void handleServerFull(serv::Instruction& instruction);
        void handleLoadScene(serv::Instruction& instruction);
        void handleStartGame(serv::Instruction& instruction);
        void handleMessage(serv::Instruction& instruction);

        void handleAssignPlayerID(serv::Instruction& instruction);
        void handlePlayerSpawn(serv::Instruction& instruction);
        void handlePlayerMoves(serv::Instruction& instruction);
        void handlePlayerShoots(serv::Instruction& instruction);
        void handlePlayerDies(serv::Instruction& instruction);

        void handleEnemySpawn(serv::Instruction& instruction);
        void handleEnemySpawn2(serv::Instruction& instruction);
        void handleEnemyMoves(serv::Instruction& instruction);
        void handleEnemyShoots(serv::Instruction& instruction);
        void handleEnemyDies(serv::Instruction& instruction);

        void handleBlockSpawn(serv::Instruction& instruction);

        void handleResetSignal(serv::Instruction& instruction);

        void handleDisconnect(serv::Instruction& instruction);

        /**
         * @brief Sens the I_PLAYER_MOVES instruction to the server
         *
         */
        void sendPlayerMoves(int id);

        /**
         * @brief Sends the I_AM_ALIVE instruction to the server.
         *
         */
        void pingServerForAlive();

        /**
         * @brief Loads the correct animation for the player depending on his/her id.
         *
         */
        void setPlayerAnimation(int id, int entity);

        std::thread _pingThread;

        bool _isConnected = false;
        bool _startGame = false;
        std::string _currSceneName = "";

        eng::Engine* _engine = nullptr;

        int _playerId = 0;
        int _entityID;
        bool _isAssignedLocalPlayer = false;

        std::map<int, std::shared_ptr<PlayerFromServerController>>
            _players;
        std::map<int, int> _enemies;

        std::shared_ptr<eng::Observer> _observer;

        bool _reset = false;

        std::map<int, void (RTypeDistantServer::*)(serv::Instruction&)>
            _requestHandlers = {
                { serv::I_CONNECT_OK, &RTypeDistantServer::handleConnectOk },
                { serv::E_SERVER_FULL, &RTypeDistantServer::handleServerFull },
                { serv::I_LOAD_SCENE, &RTypeDistantServer::handleLoadScene },
                { serv::I_START_GAME, &RTypeDistantServer::handleStartGame },
                { serv::I_MESSAGE, &RTypeDistantServer::handleMessage },
                { serv::E_INVALID_OPCODE, nullptr },

                { eng::RType::I_PLAYER_ID, &RTypeDistantServer::handleAssignPlayerID },
                { eng::RType::I_PLAYER_SPAWN, &RTypeDistantServer::handlePlayerSpawn },
                { eng::RType::I_PLAYER_MOVES, &RTypeDistantServer::handlePlayerMoves },
                { eng::RType::I_PLAYER_DIES, &RTypeDistantServer::handlePlayerDies },
                { eng::RType::I_PLAYER_SHOOTS, &RTypeDistantServer::handlePlayerShoots },

                { eng::RType::I_ENEMY_SPAWN, &RTypeDistantServer::handleEnemySpawn },
                { eng::RType::I_ENEMY_SPAWN2, &RTypeDistantServer::handleEnemySpawn2 },
                { eng::RType::I_ENEMY_MOVES, &RTypeDistantServer::handleEnemyMoves },
                { eng::RType::I_ENEMY_DIES, &RTypeDistantServer::handleEnemyDies },
                { eng::RType::I_ENEMY_SHOOTS, &RTypeDistantServer::handleEnemyShoots },

                { eng::RType::I_BLOCK_SPAWN, &RTypeDistantServer::handleBlockSpawn },

                { eng::RType::I_RESET_CLIENT, &RTypeDistantServer::handleResetSignal },

                { serv::I_DISCONNECT, &RTypeDistantServer::handleDisconnect },
            };

        int _killCount = 0;
    };
} // namespace rtype
