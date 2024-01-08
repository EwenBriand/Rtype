/*
** EPITECH PROJECT, 2023
** safe
** File description:
** LocalCarController.cpp
*/

#include "LocalCarController.hpp"
#include "ECSImpl.hpp"

// ====================================================================
// IController methods
// ====================================================================

std::vector<std::string>& LocalCarController::GetDirectives()
{
    return _directives;
}

void LocalCarController::PollDirectives()
{
    _directives.clear();
    for (auto& directive : _directivesTests) {
        if ((this->*directive.second)()) {
            _directives.push_back(directive.first);
        }
    }
}

// ====================================================================
// public methods
// ====================================================================

// ====================================================================
// private methods
// ====================================================================

bool LocalCarController::testLeft()
{
    return SYS.GetInputManager().isDown(KEY_A);
}

bool LocalCarController::testRight()
{
    return SYS.GetInputManager().isDown(KEY_D);
}

bool LocalCarController::testForward()
{
    return SYS.GetInputManager().isDown(KEY_W);
}

bool LocalCarController::testBackward()
{
    return SYS.GetInputManager().isDown(KEY_S);
}

bool LocalCarController::testBrake()
{
    return SYS.GetInputManager().isDown(KEY_SPACE);
}