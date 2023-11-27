/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CoreTransform.cpp
*/

#include "CoreTransform.hpp"

void CoreTransform::Update(__attribute__((unused)) int entityID)
{
}

graph::vec2f CoreTransform::GetScreenPosition() const
{
    return {x, y};
}

graph::vec3f CoreTransform::GetWorldPosition() const
{
    return {x, y, z};
}
