/*
** EPITECH PROJECT, 2023
** safe
** File description:
** GamePool.cpp
*/

#include "GamePool.hpp"
#include "Engine.hpp"

extern "C" {
std::shared_ptr<eng::IGame> create()
{
    std::cout << "Loading game_pool" << std::endl;
    return std::make_shared<eng::GamePool>();
}
}

namespace eng {

    // =========================================================================================================== //
    // =============================================== PUBLIC ==================================================== //
    // =========================================================================================================== //

    void GamePool::Init(Engine* engine)
    {
        _engine = engine;
    }

    void GamePool::Cleanup(Engine*)
    {
    }

    bool GamePool::IsOnLine(Engine*)
    {
        bool returnValue;
        return returnValue;
    }

    void GamePool::WaitConnect(Engine*)
    {
        initNetwork();
    }

    void GamePool::LoadFirstScene(Engine*)
    {
    }

    void GamePool::PreSceneInstantiationHook(Engine*, const std::string& sceneName)
    {
    }

    void GamePool::ModPipeline(Engine*)
    {
    }

    // =========================================================================================================== //
    // ============================================== PRIVATE ==================================================== //
    // =========================================================================================================== //

    void GamePool::initNetwork()
    {
        if (_engine->IsServer())
            initServer();
        else if (_engine->IsClient())
            initClient();
    }

    void GamePool::initServer()
    {
        _server = &_engine->GetServer();
        // start states for server here
        // init coroutines, etc...
    }

    void GamePool::initClient()
    {
        _client = &_engine->GetClient();
        // start states for client here
        // init coroutines, etc...
    }
}