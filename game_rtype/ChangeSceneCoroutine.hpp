/*
** EPITECH PROJECT, 2023
** merging
** File description:
** ChangeSceneCoroutine.hpp
*/

#pragma once

#include "Coroutine.hpp"
#include "States.hpp"
#include <string>

namespace rtype {
    /**
     * @brief A Coroutine that changes the scene. It inherits from the IState interface,
     * and must be constructed with the next state to call after the scene is changed.
     * It can be used in both the server and the client.
     *
     */
    class ChangeSceneCoroutine : public ecs::IState {
    public:
        ChangeSceneCoroutine(const std::string& sceneName, std::shared_ptr<ecs::IState> nextState);
        ~ChangeSceneCoroutine();

        void Enter() override;
        std::shared_ptr<ecs::IState> Exit(bool& changed) override;

    private:
        std::string _sceneName;
        std::shared_ptr<ecs::IState> _nextState;
        serv::Coroutine _routine;
        serv::Coroutine runClient();
        serv::Coroutine runServer();
    };
} // namespace rtype
