/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AActor.cpp
*/

#include "IActor.hpp"

namespace ecs {
    void AActor::Possess(int entity, std::shared_ptr<IController> controller)
    {
        _controller = controller;
        _controller->SetEntity(entity);
    }
} // namespace ecs
