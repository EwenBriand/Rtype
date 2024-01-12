/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CoreTransform.hpp
*/

#ifndef FCA93E06_260A_4A6F_987B_B0613F83E883
#define FCA93E06_260A_4A6F_987B_B0613F83E883
#include "../Component.hpp"
#include "../IGraphicalModule.hpp"
#include "luaScriptingAPI.hpp"
#include "metadata.hpp"
#include <iostream>

_LUAC serialize class CoreTransform : public ecs::Component<CoreTransform> {
public:
    GENERATE_METADATA(CoreTransform)
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
    graph::vec3f GetForward() const;
};

#endif /* FCA93E06_260A_4A6F_987B_B0613F83E883 */
