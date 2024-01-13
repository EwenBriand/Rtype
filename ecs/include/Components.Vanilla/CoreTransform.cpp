/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CoreTransform.cpp
*/

#include "CoreTransform.hpp"
#include "IGraphicalModule.hpp"
#include <cmath>

void CoreTransform::Update(__attribute__((unused)) int entityID)
{
}

graph::vec2f CoreTransform::GetScreenPosition() const
{
    return { x, y };
}

graph::vec3f CoreTransform::GetWorldPosition() const
{
    return { x, y, z };
}

graph::vec3f CoreTransform::GetForward() const
{
    return { cosf(rotationY), sinf(rotationY), 0 };
}