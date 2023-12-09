/*
** EPITECH PROJECT, 2022
** ecs
** File description:
** ECS.cpp
*/

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
        while (_running) {
            preUpdate();
            Update();
            postUpdate();
            _skipFrame = false;
        }
    }
}
