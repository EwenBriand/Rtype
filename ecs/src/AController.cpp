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
} // namespace ecs