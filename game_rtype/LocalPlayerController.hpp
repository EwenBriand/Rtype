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

    inline void SetPlayerId(int id) { _playerId = id; }

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
    int _playerId = -1;

    std::vector<std::string> _directives;
    std::map<std::string, bool (LocalPlayerController::*)()> _directivesTests = {
        { Ship::COMMAND_UP, &LocalPlayerController::testUp },
        { Ship::COMMAND_DOWN, &LocalPlayerController::testDown },
        { Ship::COMMAND_LEFT, &LocalPlayerController::testLeft },
        { Ship::COMMAND_RIGHT, &LocalPlayerController::testRight },
        { Ship::COMMAND_SHOOT, &LocalPlayerController::testShoot }
    };
};