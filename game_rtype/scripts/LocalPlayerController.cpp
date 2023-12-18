/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LocalPlayerController.cpp
*/

#include "LocalPlayerController.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(LocalPlayerController)

// ====================================================================
// Component methods
// ====================================================================

void LocalPlayerController::Start()
{
}

void LocalPlayerController::OnAddComponent(int e)
{
}

void LocalPlayerController::Update(int e)
{
}

// ====================================================================
// IController methods
// ====================================================================

std::vector<std::string>& LocalPlayerController::GetDirectives()
{
    return _directives;
}

void LocalPlayerController::PollDirectives()
{
    _directives.clear();
    for (auto& [directive, test] : _directivesTests) {
        if ((this->*test)())
            _directives.push_back(directive);
    }
}

// ====================================================================
// public methods
// ====================================================================

LocalPlayerController::~LocalPlayerController()
{
}

// ====================================================================
// getters/setters
// ====================================================================

// ====================================================================
// private methods
// ====================================================================

bool LocalPlayerController::testUp()
{
    return SYS.GetInputManager().isDown(KEY_UP);
}

bool LocalPlayerController::testDown()
{
    return SYS.GetInputManager().isDown(KEY_DOWN);
}

bool LocalPlayerController::testLeft()
{
    return SYS.GetInputManager().isDown(KEY_LEFT);
}

bool LocalPlayerController::testRight()
{
    return SYS.GetInputManager().isDown(KEY_RIGHT);
}

bool LocalPlayerController::testShoot()
{
    return SYS.GetInputManager().MouseButtonPressed(MOUSE_BUTTON_LEFT);
}
