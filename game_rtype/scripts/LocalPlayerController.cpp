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
    return false;
}

bool LocalPlayerController::testDown()
{
    return false;
}

bool LocalPlayerController::testLeft()
{
    return false;
}

bool LocalPlayerController::testRight()
{
    return false;
}

bool LocalPlayerController::testShoot()
{
    return false;
}
