#include "ClientUDP.hpp"
#include "IGame.hpp"
#include "ServerUdp.hpp"
#include "States.hpp"

#pragma once
namespace eng {
    class Engine;
    class RType : public eng::IGame {
    public:
        /**
         * @brief Resets the client and returns it to the menu.
         *
         */
        static constexpr int I_RESET_CLIENT = 15000;

        /**
         * @brief Assigns an id to a player.
         * args: {int id}
         *
         */
        static constexpr int I_PLAYER_ID = 5000;

        /**
         * @brief args: {int nameLen, string prefabName, int id, int x, int y}
         *
         */
        static constexpr int I_ENEMY_SPAWN = 5001;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_ENEMY_MOVES = 5002;

        /**
         * @brief args: {int id}
         *
         */
        static constexpr int I_ENEMY_DIES = 5003;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_ENEMY_SHOOTS = 5004;

        /**
         * @brief Indicates that a player has spawned.
         *
         * args: {int id, int x, int y}
         */
        static constexpr int I_PLAYER_SPAWN = 5005;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_PLAYER_SHOOTS = 5006;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_PLAYER_MOVES = 5007;

        /**
         * @brief args: {int id}
         *
         */
        static constexpr int I_PLAYER_DIES = 5008;

        RType() = default;
        ~RType() = default;

        void Init(Engine*) override;
        void Cleanup(Engine*) override;
        bool IsOnLine(Engine*) override;
        void WaitConnect(Engine*) override;
        void LoadFirstScene(Engine*) override;
        void PreSceneInstantiationHook(Engine*, const std::string& sceneName) override;
        void ModPipeline(Engine*) override;

    private:
        /**
         * @brief Loads any dependencies that the game might need
         *
         */
        void loadDependencies(Engine*);

        /**
         * @brief Inits the game's connection to the server, or the server itself
         * if this instance of the game is the server.
         *
         */
        void initServerConnection(Engine*);

        void startServer(Engine* e);
        void connectToServer(Engine* e);

        bool m_isServer = false;
        std::shared_ptr<serv::ServerUDP> m_server = nullptr;
        std::shared_ptr<serv::ClientUDP> m_client = nullptr;

        ecs::States _stateMachine;
    };
} // namespace eng