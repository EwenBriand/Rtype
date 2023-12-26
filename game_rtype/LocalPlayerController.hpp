/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LocalPlayerController.hpp
*/

#pragma once
#include "./Ship.hpp"
#include "IController.hpp"

class LocalPlayerController : public ecs::AController {
public:
    // ====================================================================
    // IController methods
    // ====================================================================

    std::vector<std::string>& GetDirectives() override;
    void PollDirectives() override;

    // ====================================================================
    // public methods
    // ====================================================================

    // ====================================================================
    // getters/setters
    // ====================================================================

private:
    // ====================================================================
    // private methods
    // ====================================================================

    bool testUp();
    bool testDown();
    bool testLeft();
    bool testRight();
    bool testShoot();

    // ====================================================================
    // properties
    // ====================================================================
    std::vector<std::string> _directives;
    std::map<std::string, bool (LocalPlayerController::*)()> _directivesTests = {
        { Ship::COMMAND_UP, &LocalPlayerController::testUp },
        { Ship::COMMAND_DOWN, &LocalPlayerController::testDown },
        { Ship::COMMAND_LEFT, &LocalPlayerController::testLeft },
        { Ship::COMMAND_RIGHT, &LocalPlayerController::testRight },
        { Ship::COMMAND_SHOOT, &LocalPlayerController::testShoot }
    };
};