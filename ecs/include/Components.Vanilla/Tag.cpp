/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Tag.cpp
*/

#include "Tag.hpp"
#include <algorithm>
#include <cctype>

const std::string& Tag::Get() const
{
    return name;
}

void Tag::Set(const std::string& name)
{
    this->name = name;
}