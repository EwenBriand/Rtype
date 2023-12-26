/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AController.cpp
*/

#include "Engine.hpp"
#include "IController.hpp"

namespace ecs {
    AController::AController()
    {
        UpdatePipeline();
    }

    void AController::UpdatePipeline()
    {
        eng::Engine::GetEngine()->pushPipeline([this]() {
            PollDirectives();
        },
            -600);
    }

    void AController::SetEntity(int entityID)
    {
        _entity = entityID;
    }

    int AController::GetEntity() const
    {
        return _entity;
    }
} // namespace ecs