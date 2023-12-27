/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AIController.hpp
*/

#pragma once
#include "./Ship.hpp"
#include "IController.hpp"
#include "Observer.hpp"
#include "Timer.hpp"

namespace rtype {
    class AIController : public ecs::AController {
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

        void SetID(int id);
        int GetID() const;
        void SetObserver(std::shared_ptr<eng::Observer> observer);

    private:
        // ====================================================================
        // private methods
        // ====================================================================
        bool testLeft();
        bool testShoot();

        // ====================================================================
        // properties
        // ====================================================================

        eng::Timer _shootTimer;
        float _shootInterval = 1.0f;
        int _id;
        std::shared_ptr<eng::Observer> _observer;
        std::vector<std::string> _directives;
        std::map<std::string, bool (AIController::*)()> _directivesTests = {
            { Ship::COMMAND_LEFT, &AIController::testLeft },
            { Ship::COMMAND_SHOOT, &AIController::testShoot }
        };
    };
} // namespace rtype