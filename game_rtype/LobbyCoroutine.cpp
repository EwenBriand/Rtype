/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LobbyCoroutine.cpp
*/

#include "LobbyCoroutine.hpp"

namespace rtype {

    // ================================================================================
    // SERVER
    // ================================================================================

    LobbyRoutineServer::LobbyRoutineServer(eng::Engine& engine)
        : _engine(engine)
        , _routine(run())
    {
    }

    LobbyRoutineServer::~LobbyRoutineServer()
    {
    }

    void LobbyRoutineServer::Enter()
    {
        _routine.Resume();
    }

    std::shared_ptr<ecs::IState> LobbyRoutineServer::Exit()
    {
        return std::shared_ptr<ecs::IState>(this);
    }

    serv::Coroutine LobbyRoutineServer::run()
    {
        while (true) {
            // @laurent logic goes here
            co_await std::suspend_never {};
        }
    }

    // ================================================================================
    // CLIENT
    // ================================================================================

    LobbyRoutineClient::LobbyRoutineClient(eng::Engine& engine)
        : _engine(engine)
        , _routine(run())
    {
    }

    LobbyRoutineClient::~LobbyRoutineClient()
    {
    }

    void LobbyRoutineClient::Enter()
    {
    }

    std::shared_ptr<ecs::IState> LobbyRoutineClient::Exit()
    {
        return std::shared_ptr<ecs::IState>(this);
    }

    serv::Coroutine LobbyRoutineClient::run()
    {
        while (true) {
            // @l aurent logic goes here
            co_await std::suspend_never {};
        }
    }

}