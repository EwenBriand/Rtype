/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CoreTransform.hpp
*/

#ifndef FCA93E06_260A_4A6F_987B_B0613F83E883
#define FCA93E06_260A_4A6F_987B_B0613F83E883
#pragma once
#include "../Component.hpp"
#include "../IGraphicalModule.hpp"
#include "luaScriptingAPI.hpp"
#include "metadata.hpp"
#include <iostream>

_LUAC serialize class CoreTransform : public ecs::Component<CoreTransform> {
public:
    GENERATE_METADATA(CoreTransform)
    _LUAM float SetX(float x);
    _LUAM float SetY(float y);
    _LUAM float SetZ(float z);

    _LUAM float SetRotationX(float x);
    _LUAM float SetRotationY(float y);
    _LUAM float SetRotationZ(float z);

    _LUAM float SetScaleX(float x);
    _LUAM float SetScaleY(float y);
    _LUAM float SetScaleZ(float z);

    _LUAM float GetX() const;
    _LUAM float GetY() const;
    _LUAM float GetZ() const;

    _LUAM float GetRotationX() const;
    _LUAM float GetRotationY() const;
    _LUAM float GetRotationZ() const;

    _LUAM float GetScaleX() const;
    _LUAM float GetScaleY() const;
    _LUAM float GetScaleZ() const;

    serialize float x = 0;
    serialize float y = 0;
    serialize float z = 0;

    serialize float rotationX = 0;
    serialize float rotationY = 0;
    serialize float rotationZ = 0;

    serialize float scaleX = 1;
    serialize float scaleY = 1;
    serialize float scaleZ = 1;

    void Update(int entityID);

    graph::vec2f GetScreenPosition() const;
    graph::vec3f GetWorldPosition() const;
};

#endif /* FCA93E06_260A_4A6F_987B_B0613F83E883 */
