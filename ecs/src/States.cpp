/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** States.cpp
*/

#include "States.hpp"

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
        auto nextState = _currentState->Exit();
        if (nextState)
            _currentState = nextState;
    }
} // namespace ecs
