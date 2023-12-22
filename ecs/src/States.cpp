/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** States.cpp
*/

#include "States.hpp"
#include <iostream>

namespace ecs {
    States::States()
        : _currentState(nullptr)
    {
    }

    States::States(std::shared_ptr<IState> initialState)
        : _currentState(initialState)
    {
    }

    States::~States()
    {
    }

    void States::SetState(std::shared_ptr<IState> state)
    {
        _currentState = state;
    }

    void States::Iter()
    {
        if (!_currentState)
            return;
        _currentState->Enter();
        bool changed = false;
        auto nextState = _currentState->Exit(changed);
        if (changed && nextState) {
            _currentState = nextState;
        }
    }
} // namespace ecs
