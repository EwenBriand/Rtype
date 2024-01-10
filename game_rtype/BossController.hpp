/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossController.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "IController.hpp"
#include "IGraphicalModule.hpp"
#include "ServerUdp.hpp"
#include "Timer.hpp"
#include <random>

namespace rtype {
    class BossController : public ecs::AController {
    public:
        BossController();
        ~BossController() = default;

        void PollDirectives();
        std::vector<std::string>& GetDirectives();
        void SetEntityID(int entityID);

    private:
        bool moveUp();
        bool moveDown();
        bool moveLeft();
        bool moveRight();
        bool shoot();

        void getTargetPosition();
        void broadcastNewTargetPosition();

        std::map<std::string, bool (BossController::*)()> _directivesTests = {
            { "up", &BossController::moveUp },
            { "down", &BossController::moveDown },
            { "left", &BossController::moveLeft },
            { "right", &BossController::moveRight },
            // { "shoot", &BossController::shoot }
        };
        graph::vec2i _targetPosition;
        graph::vec2i _currentPosition;
        std::mt19937 _generator;
        int _entityID;
        eng::Timer _shootTimer;
        std::uniform_int_distribution<int> _distribution;
        std::vector<std::string> _directives;
    };
}
