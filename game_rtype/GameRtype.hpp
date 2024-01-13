#include "ClientUDP.hpp"
#include "IGame.hpp"
#include "ServerUdp.hpp"
#include "States.hpp"

#pragma once
namespace eng {
    class Engine;

    struct SessionData {
        int killCount = 0;
    };

    class RType : public eng::IGame {
    public:
        enum class BonusType {
            HEAL,
            X2,
            X3,
            TCEMORT,
        };

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

        /**
         * @brief args: {int nameLen, string prefabName, int id, int x, int y}
         *
         */
        static constexpr int I_ENEMY_SPAWN2 = 5009;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_ENEMY_VELOCITY = 5010;

        /**
         * @brief a bonus spawn at a specific position args: {int type enemy, int bonusType, int x, int y}
         *
         */
        static constexpr int I_BONUS_SPAWN = 5011;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_PLAYER_SHOOTS_TCEMORT = 5012;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_FORCE_SHOOTS_TCEMORT = 5014;

        /**
         * @brief args: {int id, int x, int y}
         *
         */
        static constexpr int I_FORCE_SPAWN = 5013;

        /**
         * @brief args: {int entity, int x, int y}
         *
         */
        static constexpr int I_FORCE_MOVES = 5015;

        /**
         * @brief args: {int entity, int x, int y}
         *
         */
        static constexpr int I_FORCE_SHOOTS = 5016;

        /**
         * @brief args: {int entity, int x, int y}
         *
         */
        static constexpr int I_BITS_SPAWN = 5017;

        /**
         * @brief The number of enemies to kill for the game to end
         *
         */
        static constexpr int KILL_COUNT_TO_END = 30;

        /**
         * @brief Indicates a new position for the boss to move to.
         * args: {int x, int y}
         *
         */
        static constexpr int I_BOSS_MOVES = 6013;

        /**
         * @brief Indicates that the boss has spawned.
         *
         */
        static constexpr int I_BOSS_SPAWNS = 6014;

        /**
         * @brief Indicates that the boss just shot from coordinates (x, y)
         *
         */
        static constexpr int I_BOSS_SHOOTS = 85236;

        /*  */
        static constexpr int I_LEVEL2 = 8000;

        static constexpr int I_BLOCK_SPAWN = 6969;

        RType() = default;

        ~RType() = default;

        void Init(Engine*) override;
        void Cleanup(Engine*) override;
        bool IsOnLine(Engine*) override;
        void WaitConnect(Engine*) override;
        void LoadFirstScene(Engine*) override;
        void PreSceneInstantiationHook(Engine*, const std::string& sceneName) override;
        void ModPipeline(Engine*) override;

        inline SessionData& GetSessionData() { return _sessionData; }

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

        SessionData _sessionData;

        bool m_isServer = false;
        std::shared_ptr<serv::ServerUDP> m_server = nullptr;
        std::shared_ptr<serv::ClientUDP> m_client = nullptr;

        ecs::States _stateMachine;

        int _kill_count = 0;
    };
} // namespace eng
