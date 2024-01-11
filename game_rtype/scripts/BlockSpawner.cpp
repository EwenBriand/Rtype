/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** BlockSpawner.cpp
*/

#include "BlockSpawner.hpp"
#include "AIController.hpp"
#include "Block.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "IGraphicalModule.hpp"
#include "ServerUdp.hpp"
#include "metadata.hpp"
#include <iostream>
#include <random>
#include <vector>

MANAGED_RESOURCE(BlockSpawner)

unsigned int BlockSpawner::id = 2000;

BlockSpawner::~BlockSpawner()
{
}

void BlockSpawner::Update(int e)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        std::cout << "caution, engine not in play mode" << std::endl;
    if (not eng::Engine::GetEngine()->IsServer())
        return;

    if (_timer.GetElapsedTime() < _spawnDelay + _randomDelayVariation or _transform == nullptr
        or not eng::Engine::GetEngine()->PlayMode())
        return;

     try {
        graph::vec2i size = SYS.GetGraphicalModule()->GetScreenSize();
        std::uniform_int_distribution<int> dist(0, size.x);
        _transform->x = dist(_rng);

        // if (1) {
            // if there is less that 2 block on the screen then spawn one
        spawnBlock();
        _timer.Restart();
        // } else {
            // _spawnDelay = 10.0f;
        //     std::random_device rd;
        //     std::mt19937 gen(rd());
        //     std::uniform_int_distribution<> distrib(0, 1);
        //     (distrib(gen) == 0) ? spawnEnemy1() : spawnEnemy2();
        // }
    } catch (const std::exception& e) {
        CONSOLE::err << "\rCould not spawn Block: \n\r\t" << e.what() << std::endl;
    }

}

void BlockSpawner::spawnBlock()
{
    unsigned int e = SYS.GetResourceManager().LoadPrefab(_prefabName);
    auto& transform = SYS.GetComponent<CoreTransform>(e);
    transform.x = _transform->x;
    transform.y = _transform->y;

    // auto& ship = SYS.GetComponent<Block>(e, "Block");

    // auto controller = std::make_shared<rtype::AIController>();
    // if (not controller)
    //     throw std::runtime_error("Could not create AIController");
    // controller->SetID(id++);
    // ship.SetID(controller->GetID());
    // ship.Possess(e, controller);
    // setupObserver(controller, e);
    broadcastSpawn(transform.x, transform.y, _prefabName);
}

void BlockSpawner::OnAddComponent(int e)
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

void BlockSpawner::Start()
{
}

void BlockSpawner::OnLoad()
{
}

void BlockSpawner::broadcastSpawn(int x, int y, std::string prefabName)
{
    auto* engine = eng::Engine::GetEngine();

    if (not engine->IsServer())
        return;
    serv::bytes args(std::vector<int>({ x, y }));
    args += serv::bytes("Block");
    serv::Instruction instruction;
    instruction = serv::Instruction(eng::RType::I_BLOCK_SPAWN, 0, args);
    engine->GetServer().Broadcast(instruction.ToBytes() + serv::SEPARATOR);
}