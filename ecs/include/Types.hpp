/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Types.hpp
*/

#pragma once

#include <iostream>
#include <functional>
#include "metadata.hpp"

using Action = std::function<void()>;
using CptMetadata = std::map<std::string, metadata_t>;
