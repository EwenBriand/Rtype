/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LobbyCoroutine.cpp
*/

#include "LobbyCoroutine.hpp"
#include "DistantPlayer.hpp"
#include "GameRoutine.hpp"
#include "RTypeDistantServer.hpp"
#include "Timer.hpp"
#include <chrono>

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
        if (!_routine.Done()) {
            _routine.Resume();
        }
    }

    std::shared_ptr<ecs::IState> LobbyRoutineServer::Exit(bool& changed)
    {
        if (_routine.Done()) {
            return std::shared_ptr<ecs::IState>(new GameRoutineServer(_engine));
        }
        changed = false;
        return nullptr;
    }

    serv::Coroutine LobbyRoutineServer::run()
    {
        while (!lobbyIsFull()) {
            co_await std::suspend_always {};
        }
        std::cout << "Lobby is full, starting game..." << std::endl;
        for (auto& player : DistantPlayer::Instances) {
            player->SendClientLoadScene("level1");
        }

        int playerReady = 0;
        while (playerReady != RTYPE_NB_PLAYERS) {
            playerReady = 0;
            for (auto& player : DistantPlayer::Instances) {
                if (player->GetAnswerFlag()) {
                    playerReady++;
                }
            }
            co_await std::suspend_always {};
        }
        for (auto& player : DistantPlayer::Instances) {
            player->ResetAnswerFlag();
        }
    }

    bool LobbyRoutineServer::lobbyIsFull()
    {
        if (DistantPlayer::Instances.size() != _nbPlayers) {
            _nbPlayers = DistantPlayer::Instances.size();
        }
        return _nbPlayers == RTYPE_NB_PLAYERS;
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
        if (!_routine.Done()) {
            _routine.Resume();
        }
    }

    std::shared_ptr<ecs::IState> LobbyRoutineClient::Exit(bool& changed)
    {
        changed = false;
        return nullptr;
    }

    serv::Coroutine LobbyRoutineClient::run()
    {
        auto serverHandle = RTypeDistantServer::GetInstance();
        eng::Timer timer;
        timer.Start();

        serverHandle.TryConnect();
        while (not serverHandle.IsConnected()) {
            if (timer.GetElapsedTime() > 1) {
                timer.Restart();
                serverHandle.TryConnect();
            }
            co_await std::suspend_always {};
        }
        std::cout << "Connected to server" << std::endl;
    }
}