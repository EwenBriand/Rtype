/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** EnemySpawner.cpp
*/

#include "EnemySpawner.hpp"
#include "AIController.hpp"
#include "Enemy.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "IGraphicalModule.hpp"
#include "ServerUdp.hpp"
#include "metadata.hpp"
#include <iostream>
#include <random>
#include <vector>

MANAGED_RESOURCE(EnemySpawner);

unsigned int EnemySpawner::id = 1000;

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::Update(int e)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        std::cout << "caution, engine not in play mode" << std::endl;
    if (not eng::Engine::GetEngine()->IsServer())
        return;
    if (_timer.GetElapsedTime() < _spawnDelay + _randomDelayVariation or _transform == nullptr
        or not eng::Engine::GetEngine()->PlayMode())
        return;
    _timer.Restart();
    if (_wave == 0 && _wave_enemy.size() > 0) {
        _wave = _wave_enemy[0];
        _wave_enemy.erase(_wave_enemy.begin());
    }
    if (_wave > 0) {
        _wave--;
        try {
            graph::vec2i size = SYS.GetGraphicalModule()->GetScreenSize();
            std::uniform_int_distribution<int> dist(0, size.y);
            _transform->y = dist(_rng);

            unsigned int e = SYS.GetResourceManager().LoadPrefab(_prefabName);
            auto& transform = SYS.GetComponent<CoreTransform>(e);
            transform.x = _transform->x;
            transform.y = _transform->y;

            auto& ship = SYS.GetComponent<Enemy>(e, "Enemy");
            auto controller = std::make_shared<rtype::AIController>();
            if (not controller)
                throw std::runtime_error("Could not create AIController");
            controller->SetID(id++);
            ship.SetID(controller->GetID());
            ship.Possess(e, controller);
            setupObserver(controller, e);
            broadcastSpawn(*controller, transform.x, transform.y);
        } catch (const std::exception& e) {
            CONSOLE::err << "\rCould not spawn enemy: \n\r\t" << e.what() << std::endl;
        }
    }
}

void EnemySpawner::OnAddComponent(int e)
{
    std::cout << "add enemy spawner" << std::endl;
    if (not eng::Engine::GetEngine()->IsServer())
        return;
    try {
        _rng = std::mt19937 { std::random_device {}() };
        _transform = &SYS.SafeGet<CoreTransform>(e);
        std::uniform_real_distribution<float> dist(-_maxRandomDelayVariation, _maxRandomDelayVariation);
        _randomDelayVariation = dist(_rng);
        _timer.Start();
    } catch (const std::exception& e) {
        CONSOLE::err << "Could not get transform\n\r\t" << e.what() << std::endl;
    }
}

void EnemySpawner::Start()
{
}

void EnemySpawner::OnLoad()
{
}

void EnemySpawner::broadcastSpawn(const rtype::AIController& controller, int x, int y)
{
    auto* engine = eng::Engine::GetEngine();

    if (not engine->IsServer())
        return;
    serv::bytes args(std::vector<int>({ controller.GetID(), x, y }));
    args += serv::bytes(_prefabName);
    auto instruction = serv::Instruction(eng::RType::I_ENEMY_SPAWN, 0, args);
    engine->GetServer().Broadcast(instruction.ToBytes() + serv::SEPARATOR);
}

void EnemySpawner::setupObserver(std::shared_ptr<rtype::AIController> controller, int e)
{
    std::shared_ptr<eng::Observer> observer = eng::Engine::GetEngine()->RegisterObserver();
    // try {
    //     auto& tr = SYS.GetComponent<CoreTransform>(e);
    //     observer->RegisterTarget([e, controller]() {
    //         if (not eng::Engine::GetEngine()->IsServer())
    //             return;
    //         auto& server = eng::Engine::GetEngine()->GetServer();

    //         try {
    //             auto& tr = SYS.GetComponent<CoreTransform>(e);
    //             int id = controller->GetID();
    //             int x = tr.x;
    //             int y = tr.y;
    //             serv::bytes data(std::vector<int>({ id, x, y }));
    //             server.Broadcast(serv::Instruction(eng::RType::I_ENEMY_MOVES, 0, data));
    //         } catch (const std::exception& e) {
    //             CONSOLE::err << "Could not broadcast enemy moves\n\r\t" << e.what() << std::endl;
    //         }
    //     },
    //         tr.x, tr.y);
    // } catch (const std::exception& e) {
    //     CONSOLE::err << "Could not setup observer\n\r\t" << e.what() << std::endl;
    // }
}
