/*
** EPITECH PROJECT, 2023
** merging
** File description:
** ChangeSceneCoroutine.cpp
*/

#include "ChangeSceneCoroutine.hpp"
#include "DistantPlayer.hpp"
#include "Engine.hpp"
#include "NetworkExceptions.hpp"
#include "RTypeDistantServer.hpp"

namespace rtype {
    ChangeSceneCoroutine::ChangeSceneCoroutine(const std::string& sceneName, std::shared_ptr<ecs::IState> nextState)
        : _nextState(nextState)
        , _routine((eng::Engine::GetEngine()->IsServer()) ? runServer() : runClient())
        , _sceneName(sceneName)
    {
    }

    ChangeSceneCoroutine::~ChangeSceneCoroutine()
    {
    }

    void ChangeSceneCoroutine::Enter()
    {
        if (!_routine.Done()) {
            _routine.Resume();
        }
    }

    std::shared_ptr<ecs::IState> ChangeSceneCoroutine::Exit(bool& changed)
    {
        if (_routine.Done()) {
            changed = true;
            return _nextState;
        }
        changed = false;
        return nullptr;
    }

    serv::Coroutine ChangeSceneCoroutine::runServer()
    {
        for (auto& player : DistantPlayer::Instances) {
            player->ResetAnswerFlag();
            player->SendClientLoadScene(_sceneName);
        }
        int playerReady = 0;
        while (playerReady < DistantPlayer::Instances.size()) {
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

    serv::Coroutine ChangeSceneCoroutine::runClient()
    {
        RTypeDistantServer* serverHandle = RTypeDistantServer::GetInstance();
        while (not serverHandle->SceneIsReady()) {
            co_await std::suspend_always {};
        }
        serverHandle->InstantiateScene();
        eng::Engine::GetEngine()->GetClient().Send(serv::Instruction(serv::I_OK, 0, serv::bytes()));
        CONSOLE::info << "Scene [" << _sceneName << "] : loaded" << std::endl;
    }
} // namespace rtype
