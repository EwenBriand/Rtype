/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** NamedScopeLock.hpp
*/

#pragma once
#include "Engine.hpp"
#include <mutex>

namespace eng {
    /**
     * @brief A lock that can be named. Mutexes are held within the Engine class, so
     * they are accessible from anywhere.
     *
     */
    class NamedScopeLock {
    public:
        NamedScopeLock(const std::string& name)
            : _name(name)
        {
            eng::Engine::GetEngine()->GetMutex(_name).lock();
        }

        ~NamedScopeLock()
        {
            eng::Engine::GetEngine()->GetMutex(_name).unlock();
        }

        static void* operator new(std::size_t) = delete;
        static void* operator new[](std::size_t) = delete;

    private:
        std::string _name;
    };
} // namespace eng