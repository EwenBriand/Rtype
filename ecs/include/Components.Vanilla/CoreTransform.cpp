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
    return { x, y };
}

graph::vec3f CoreTransform::GetWorldPosition() const
{
    return { x, y, z };
}

float CoreTransform::SetX(float x)
{
    this->x = x;
    return x;
}

float CoreTransform::SetY(float y)
{
    this->y = y;
    return y;
}

float CoreTransform::SetZ(float z)
{
    this->z = z;
    return z;
}

float CoreTransform::SetRotationX(float x)
{
    this->rotationX = x;
    return x;
}

float CoreTransform::SetRotationY(float y)
{
    this->rotationY = y;
    return y;
}

float CoreTransform::SetRotationZ(float z)
{
    this->rotationZ = z;
    return z;
}

float CoreTransform::SetScaleX(float x)
{
    this->scaleX = x;
    return x;
}

float CoreTransform::SetScaleY(float y)
{
    this->scaleY = y;
    return y;
}

float CoreTransform::SetScaleZ(float z)
{
    this->scaleZ = z;
    return z;
}

float CoreTransform::GetX() const
{
    return x;
}

float CoreTransform::GetY() const
{
    return y;
}

float CoreTransform::GetZ() const
{
    return z;
}

float CoreTransform::GetRotationX() const
{
    return rotationX;
}

float CoreTransform::GetRotationY() const
{
    return rotationY;
}

float CoreTransform::GetRotationZ() const
{
    return rotationZ;
}

float CoreTransform::GetScaleX() const
{
    return scaleX;
}

float CoreTransform::GetScaleY() const
{
    return scaleY;
}

float CoreTransform::GetScaleZ() const
{
    return scaleZ;
}