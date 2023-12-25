/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameRoutine.cpp
*/

#include "GameRoutine.hpp"
#include "DistantPlayer.hpp"
#include "ECSImpl.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include "Ship.hpp"

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
        co_await std::suspend_always {};
        serv::ServerUDP& server = _engine.GetServer();
        server.Log("Attributing player ids");
        assignPlayerIds();
        server.Log("Spawning players");
        for (auto& player : DistantPlayer::Instances)
            spawnPlayer(player);
        for (auto& player : DistantPlayer::Instances)
            player->SendClientStartGame();
        server.Log("Game started");

        while (true) {
            co_await std::suspend_always {};
        }
    }

    void GameRoutineServer::assignPlayerIds() const
    {
        int id = 0;

        for (auto& player : DistantPlayer::Instances) {
            player->SetID(id);
            player->Send(serv::Instruction(eng::RType::I_PLAYER_ID, 0, serv::bytes(reinterpret_cast<char*>(&id), sizeof(int))));
            id++;
        }
    }

    void GameRoutineServer::spawnPlayer(std::shared_ptr<DistantPlayer> player) const
    {
        auto& sys = _engine.GetECS();
        int id = player->GetID();
        try {
            int e = sys.GetResourceManager().LoadPrefab("ship");
            Ship& ship = sys.GetComponent<Ship>(e, "Ship");
            ship.Possess(e, player);

            auto& transform = sys.GetComponent<CoreTransform>(e);
            transform.x = 10;
            transform.y = 10 * id;
            player->Send(serv::Instruction(eng::RType::I_PLAYER_SPAWN, 0, serv::bytes(std::vector<int> { id, 10, 10 * id })));
        } catch (std::exception& e) {
            std::cerr << "\r" << e.what() << std::endl;
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
