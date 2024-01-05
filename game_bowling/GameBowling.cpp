/*
** EPITECH PROJECT, 2023
** safe
** File description:
** GameBowling.cpp
*/

#include "GameBowling.hpp"
#include "Engine.hpp"

extern "C" {
std::shared_ptr<eng::IGame> create()
{
    std::cout << "Loading game_bowling" << std::endl;
    return std::make_shared<eng::GameBowling>();
}
}

namespace eng {

    // =========================================================================================================== //
    // =============================================== PUBLIC ==================================================== //
    // =========================================================================================================== //

    void GameBowling::Init(Engine* engine)
    {
        _engine = engine;
    }

    void GameBowling::Cleanup(Engine*)
    {
    }

    bool GameBowling::IsOnLine(Engine*)
    {
        bool returnValue;
        return returnValue;
    }

    void GameBowling::WaitConnect(Engine*)
    {
        initNetwork();
    }

    void GameBowling::LoadFirstScene(Engine*)
    {
    }

    void GameBowling::PreSceneInstantiationHook(Engine*, const std::string& sceneName)
    {
    }

    void GameBowling::ModPipeline(Engine*)
    {
    }

    // =========================================================================================================== //
    // ============================================== PRIVATE ==================================================== //
    // =========================================================================================================== //

    void GameBowling::initNetwork()
    {
        if (_engine->IsServer())
            initServer();
        else if (_engine->IsClient())
            initClient();
    }

    void GameBowling::initServer()
    {
        _server = &_engine->GetServer();
        // start states for server here
        // init coroutines, etc...
    }

    void GameBowling::initClient()
    {
        _client = &_engine->GetClient();
        // start states for client here
        // init coroutines, etc...
    }
}