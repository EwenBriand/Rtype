/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LobbyCoroutine.hpp
*/

#pragma once

#include "Coroutine.hpp"
#include "Engine.hpp"
#include "States.hpp"
#include <memory>

namespace rtype {
    static constexpr unsigned int RTYPE_NB_PLAYERS = 3;

    class LobbyRoutineServer : public ecs::IState, public std::enable_shared_from_this<LobbyRoutineServer> {
    public:
        LobbyRoutineServer(eng::Engine& engine);
        ~LobbyRoutineServer();

        void Enter() override;
        std::shared_ptr<ecs::IState> Exit(bool& changed) override;

    private:
        serv::Coroutine run();

        /**
         * @brief waits for new players to connect and
         * returns true when the lobby is full.
         *
         */
        bool lobbyIsFull();

        unsigned int _nbPlayers = -1;

        serv::Coroutine _routine;
        eng::Engine& _engine;
    };

    class LobbyRoutineClient : public ecs::IState, public std::enable_shared_from_this<LobbyRoutineClient> {
    public:
        LobbyRoutineClient(eng::Engine& engine);
        ~LobbyRoutineClient();

        void Enter() override;
        std::shared_ptr<ecs::IState> Exit(bool& changed) override;

    private:
        serv::Coroutine run();

        serv::Coroutine _routine;
        eng::Engine& _engine;
    };
} // namespace rtype
