/*
** EPITECH PROJECT, 2022
** ecs
** File description:
** ECS.cpp
*/

#include "CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

namespace ecs {
    template <>
    void ECSImpl::NotifyEnginePipelineErased()
    {
        eng::Engine::GetEngine()->NotifyPipelineChange();
    }

    template <>
    void ECSImpl::RequestEngineClearPipeline()
    {
        eng::Engine::GetEngine()->ClearPipeline();
    }

    template <>
    void ECSImpl::Run(Action preUpdate, Action postUpdate)
    {
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (_running) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            _deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();

            preUpdate();
            Update();
            postUpdate();

            lastTime = currentTime;
            _skipFrame = false;
        }
    }

    template <>
    float ECSImpl::GetDeltaTime() const
    {
        return _deltaTime;
    }

    template <>
    void ECSImpl::EngineReloadEditorMode()
    {
        eng::Engine::GetEngine()->SetupEditor();
    }
}
