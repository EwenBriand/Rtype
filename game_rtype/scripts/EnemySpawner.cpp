/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** EnemySpawner.cpp
*/

#include "EnemySpawner.hpp"
#include "AIController.hpp"
#include "Enemy.hpp"
#include "Enemy2.hpp"
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

std::random_device rd; // Random device engine, usually based on /dev/urandom on UNIX-like systems
std::mt19937 gen(rd()); // Initialize Mersenne Twister random number generator with rd
std::uniform_int_distribution<> distrib(0, 99); // Define the distribution, inclusive.
std::uniform_int_distribution<> distrib2(0, 2); // Define the distribution, inclusive.

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
        // _wave_enemy.erase(_wave_enemy.begin());
    }
    if (_wave > 0) {
        _wave--;
        try {
            graph::vec2i size = SYS.GetGraphicalModule()->GetScreenSize();
            std::uniform_int_distribution<int> dist(50, size.y - 100);
            _transform->y = dist(_rng);

            if (eng::Engine::GetEngine()->GetGlobal<int>("killCount") < 5) {
                spawnEnemy1();
            } else {
                _spawnDelay = 10.0f;
                (distrib(gen) < _e_percentage) ? spawnEnemy1() : spawnEnemy2();
            }
        } catch (const std::exception& e) {
            CONSOLE::err << "\rCould not spawn enemy: \n\r\t" << e.what() << std::endl;
        }
    }
    std::cout << "kill count: " << eng::Engine::GetEngine()->GetGlobal<int>("killCount") << std::endl;
}

void EnemySpawner::spawnEnemy1()
{
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
    if (distrib(gen) <= 49)
        ship.SetBonusOnDeath(distrib2(gen));
    ship.Possess(e, controller);
    setupObserver(controller, e);
    broadcastSpawn(*controller, transform.x, transform.y, _prefabName);
}

void EnemySpawner::spawnEnemy2()
{
    std::cout << "spawn dual ship 2" << std::endl;
    unsigned int e = SYS.GetResourceManager().LoadPrefab("dual-ship2");
    auto& transform = SYS.GetComponent<CoreTransform>(e);
    transform.x = _transform->x;
    transform.y = _transform->y;

    auto& ship = SYS.GetComponent<Enemy2>(e, "Enemy2");
    auto controller = std::make_shared<rtype::AIController>();
    std::cout << "find Enemy 2" << std::endl;
    if (not controller)
        throw std::runtime_error("Could not create AIController");
    controller->SetID(id++);
    ship.SetID(controller->GetID());
    ship.SetBonusOnDeath((distrib(gen) < 70) ? 0 : 0);
    // if (distrib(gen) <= 49)
    // ship.SetBonusOnDeath((distrib(gen) < 70) ? -1 : 0);
    ship.Possess(e, controller);
    setupObserver(controller, e);
    broadcastSpawn(*controller, transform.x, transform.y, "dual-ship2");
}

void EnemySpawner::OnAddComponent(int e)
{
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

void EnemySpawner::broadcastSpawn(const rtype::AIController& controller, int x, int y, std::string prefabName)
{
    auto* engine = eng::Engine::GetEngine();

    if (not engine->IsServer())
        return;
    serv::bytes args(std::vector<int>({ controller.GetID(), x, y }));
    args += serv::bytes(prefabName);
    serv::Instruction instruction;
    if (prefabName == "dual-ship2")
        instruction = serv::Instruction(eng::RType::I_ENEMY_SPAWN2, 0, args);
    else
        instruction = serv::Instruction(eng::RType::I_ENEMY_SPAWN, 0, args);
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
