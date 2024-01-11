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
#include "NetworkExceptions.hpp"
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
        _engine.SetGlobal("PlayerID", -1);
        _engine.SetGlobal("ForceID", std::vector<int> {});
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
            changed = true;
            return std::shared_ptr<ecs::IState>(new LobbyRoutineServer(_engine));
        }
        changed = false;
        return nullptr;
    }

    static void debugSpawnBoss()
    {
        // EWEN BOSS
        // try {
        //     std::cout << "\rspawning boss" << std::endl;
        //     SYS.GetResourceManager().LoadPrefab("boss-head");
        //     eng::Engine::GetEngine()->SetGlobal<graph::vec2i>("bossTargetPosition", graph::vec2i { 0, 0 });
        //     eng::Engine::GetEngine()->SetGlobal<graph::vec2i>("bossShoot", graph::vec2i { -1, -1 });
        //     eng::Engine::GetEngine()->GetServer().Broadcast(serv::Instruction(eng::RType::I_BOSS_SPAWNS, 0, serv::bytes()));
        // } catch (const std::exception& e) {
        //     std::cerr << e.what() << std::endl;
        // }
    }

    serv::Coroutine
    GameRoutineServer::run()
    {
        co_await std::suspend_always {};
        eng::Engine::GetEngine()->GetSceneManager().UnloadScene("lobby");
        serv::ServerUDP& server = _engine.GetServer();
        server.Log("Attributing player ids");
        assignPlayerIds();
        server.Log("Spawning players");
        for (auto& player : DistantPlayer::Instances) {
            spawnPlayer(player);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        for (auto& player : DistantPlayer::Instances)
            player->SendClientStartGame();
        server.Log("Game started");

        debugSpawnBoss();

        while (true) {
            if (DistantPlayer::Instances.size() < RTYPE_NB_PLAYERS)
                break;
            co_await std::suspend_always {};
        }
        DistantPlayer::Instances.clear();
        server.Log("Game ended");
        server.ResetClients();
        eng::Engine::GetEngine()->GetSceneManager().LoadSceneAsync("menu");
        server.Log("Loading menu scene");
        while (not eng::Engine::GetEngine()->GetSceneManager().IsSceneReady("menu")) {
            co_await std::suspend_always {};
        }
        server.Log("Menu scene loaded");
        eng::Engine::GetEngine()->GetSceneManager().SwitchScene("menu");
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
            transform.x = 50;
            transform.y = 100 * id + 1;
            for (auto& p : DistantPlayer::Instances)
                p->Send(serv::Instruction(eng::RType::I_PLAYER_SPAWN, 0, serv::bytes(std::vector<int> { id, 10, 100 * id })));
            player->SetEntity(e);
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
            changed = true;
            return nullptr; // go back to menu and let it handle the rest
        }
        changed = false;
        return nullptr;
    }

    serv::Coroutine GameRoutineClient::run()
    {
        std::cout << "Enter game routine" << std::endl;
        eng::Engine::GetEngine()->GetSceneManager().UnloadScene("lobby");
        while (true) {
            if (rtype::RTypeDistantServer::Instance != nullptr and rtype::RTypeDistantServer::Instance->ShouldReset())
                break;
            checkEndGameConditions();
            co_await std::suspend_always {};
        }
        rtype::RTypeDistantServer::Instance->ResetReset();
        eng::Engine::GetEngine()->GetSceneManager().LoadSceneAsync("menu");
        std::cout << "Loading menu scene" << std::endl;
        while (not eng::Engine::GetEngine()->GetSceneManager().IsSceneReady("menu")) {
            co_await std::suspend_always {};
        }
        std::cout << "Menu scene loaded" << std::endl;
        eng::Engine::GetEngine()->GetSceneManager().SwitchScene("menu");
    }

    void GameRoutineClient::checkEndGameConditions()
    {
        try {
            auto rtype = std::dynamic_pointer_cast<eng::RType>(eng::Engine::GetEngine()->GetGame());
            if (rtype == nullptr)
                return;
            // std::cout << "ewen le debug est la: killcount " << rtype->GetSessionData().killCount << std::endl;
            if (rtype->GetSessionData().killCount >= eng::RType::KILL_COUNT_TO_END) {
                // TODO ewen: uncomment after you merged with branch game 1
                eng::Engine::GetEngine()->GetClient().Send(serv::Instruction(
                    eng::RType::I_LEVEL2, 0, serv::bytes()));
            }
        } catch (std::exception& e) {
            return;
        }
    }

} // namespace rtype
