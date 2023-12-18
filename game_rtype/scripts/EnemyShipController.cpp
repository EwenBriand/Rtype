/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** EnemyShipController.cpp
*/

#include "EnemyShipController.hpp"
#include <iostream>

// ====================================================================
// Component methods
// ====================================================================

void EnemyShipController::Start()
{
}

void EnemyShipController::OnAddComponent(int e)
{
}

void EnemyShipController::Update(int e)
{
    std::cout << "I am an enemy ship" << std::endl;
}

// ====================================================================
// IController methods
// ====================================================================

std::vector<std::string>& EnemyShipController::GetDirectives()
{
    return _directives;
}

void EnemyShipController::PollDirectives()
{
    _directives.clear();
    // todo
}

// ====================================================================
// public methods
// ====================================================================

// ====================================================================
// getters/setters
// ====================================================================

// ====================================================================
// private methods
// ====================================================================
