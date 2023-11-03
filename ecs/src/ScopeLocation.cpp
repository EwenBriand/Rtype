/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ScopeLocation.cpp
*/

#include "ScopeLocation.hpp"

namespace ecs
{
    ScopeLocation::ScopeLocation(const std::filesystem::path &path) {
        m_previousLocation = std::filesystem::current_path().string();
        if (std::filesystem::is_regular_file(path)) {
            std::filesystem::current_path(path.parent_path());
            return;
        }
        std::filesystem::current_path(path);
    }

    ScopeLocation::ScopeLocation(const std::string &path)
    {
        std::filesystem::path p(path);

        if (std::filesystem::is_regular_file(p)) {
            p = p.parent_path();
        }
        m_previousLocation = std::filesystem::current_path().string();
        std::filesystem::current_path(p);
    }

    ScopeLocation::~ScopeLocation()
    {
        std::filesystem::current_path(m_previousLocation);
    }
} // namespace ecs
