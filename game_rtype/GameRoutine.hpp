/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameRoutine.hpp
*/

#pragma once
#include "DistantPlayer.hpp"
#include "LobbyCoroutine.hpp"
#include "RTypeDistantServer.hpp"

namespace rtype {
    class GameRoutineServer : public ecs::IState {
    public:
        GameRoutineServer(eng::Engine& engine);
        ~GameRoutineServer();

        void Enter() override;
        std::shared_ptr<ecs::IState> Exit(bool& changed) override;

    private:
        serv::Coroutine run();

        /**
         * @brief Assigns a unique id to all players.
         *
         */
        void assignPlayerIds() const;

        /**
         * @brief Spawns all players in the scene and possesses them with a distant player.
         *
         * @param player
         */
        void spawnPlayer(std::shared_ptr<DistantPlayer> player) const;

        serv::Coroutine _routine;
        eng::Engine& _engine;
    };

    class GameRoutineClient : public ecs::IState {
    public:
        GameRoutineClient(eng::Engine& engine);
        ~GameRoutineClient();

        void Enter() override;
        std::shared_ptr<ecs::IState> Exit(bool& changed) override;

    private:
        serv::Coroutine run();

        serv::Coroutine _routine;
        eng::Engine& _engine;
    };
} // namespace rtype