/*
** EPITECH PROJECT, 2023
** safe
** File description:
** LocalCarController.hpp
*/

#pragma once
#include "IController.hpp"

class LocalCarController : public ecs::AController {
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
    inline void SetCarId(int id) { _carId = id; }

private:
    // ====================================================================
    // private methods
    // ====================================================================

    bool testLeft();
    bool testRight();
    bool testForward();
    bool testBackward();
    bool testBrake();
    bool testJump();

    // ====================================================================
    // properties
    // ====================================================================
    int _playerId = -1;
    int _carId = -1;

    std::vector<std::string> _directives;
    std::map<std::string, bool (LocalCarController::*)()> _directivesTests = {
        { "left", &LocalCarController::testLeft },
        { "right", &LocalCarController::testRight },
        { "forward", &LocalCarController::testForward },
        { "backward", &LocalCarController::testBackward },
        { "brake", &LocalCarController::testBrake },
        { "jump", &LocalCarController::testJump },
    };
};