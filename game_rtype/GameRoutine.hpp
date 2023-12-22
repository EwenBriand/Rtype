/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameRoutine.hpp
*/

#pragma once
#include "LobbyCoroutine.hpp"

namespace rtype {
    class GameRoutineServer : public ecs::IState {
    public:
        GameRoutineServer(eng::Engine& engine);
        ~GameRoutineServer();

        void Enter() override;
        std::shared_ptr<ecs::IState> Exit(bool& changed) override;

    private:
        serv::Coroutine run();

        serv::Coroutine _routine;
        eng::Engine& _engine;
    };

    class GameRoutineClient : public ecs::IState {
    public:
        GameRoutineClient();
        ~GameRoutineClient();

        void Enter() override;
        std::shared_ptr<ecs::IState> Exit(bool& changed) override;

    private:
        serv::Coroutine run();

        serv::Coroutine _routine;
        eng::Engine& _engine;
    };
} // namespace rtype
