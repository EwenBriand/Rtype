/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameRoutine.cpp
*/

#include "GameRoutine.hpp"
#include "DistantPlayer.hpp"

namespace rtype {

    // ================================================================================
    // Server
    // ================================================================================

    GameRoutineServer::GameRoutineServer(eng::Engine& engine)
        : _engine(engine)
        , _routine(run())
    {
    }

    GameRoutineServer::~GameRoutineServer()
    {
    }

    void GameRoutineServer::Enter()
    {
        if (!_routine.Done()) {
            _routine.Resume();
        }
    }

    std::shared_ptr<ecs::IState> GameRoutineServer::Exit(bool& changed)
    {
        if (_routine.Done()) {
            // return next state here
        }
        changed = false;
        return nullptr;
    }

    serv::Coroutine GameRoutineServer::run()
    {
        for (auto& player : DistantPlayer::Instances)
            player->SendClientStartGame();
        while (true) {
            co_await std::suspend_always {};
        }
    }

    // ================================================================================
    // Client
    // ================================================================================

    GameRoutineClient::GameRoutineClient(eng::Engine& engine)
        : _engine(engine)
        , _routine(run())
    {
    }

    GameRoutineClient::~GameRoutineClient()
    {
    }

    void GameRoutineClient::Enter()
    {
        if (!_routine.Done()) {
            _routine.Resume();
        }
    }

    std::shared_ptr<ecs::IState> GameRoutineClient::Exit(bool& changed)
    {
        if (_routine.Done()) {
            // return next state here
        }
        changed = false;
        return nullptr;
    }

    serv::Coroutine GameRoutineClient::run()
    {
        while (true) {
            co_await std::suspend_always {};
        }
    }

} // namespace rtype
