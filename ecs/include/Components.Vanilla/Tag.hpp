/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Tag.hpp
*/

#pragma once

#include "Component.hpp"
#include "metadata.hpp"
#include <string>

serialize class Tag : public ecs::Component<Tag> {
public:
    GENERATE_METADATA(Tag)
    const std::string& Get() const;
    void Set(const std::string& name);

private:
    serialize std::string name = "default";
};