/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AIController.hpp
*/

#pragma once
#include "./Enemy.hpp"
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

        AIController();

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

        /**
         * @brief Broadcasts the entity's position to all clients
         *
         */
        void broadcastPosition();

        /**
         * @brief Broadcasts the fact that the enemy shot to all clients
         *
         */
        void broadcastShoot();

        /**
         * @brief Instantiates a red laser beam
         *
         */
        void instantiateRedLaser();

        // ====================================================================
        // properties
        // ====================================================================

        eng::Timer _shootTimer;
        eng::Timer _broadcastTimer;
        float _shootInterval = 2.0f;
        int _id;
        std::shared_ptr<eng::Observer> _observer;
        std::vector<std::string> _directives;
        std::map<std::string, bool (AIController::*)()> _directivesTests = {
            { Enemy::COMMAND_LEFT, &AIController::testLeft },
            { Enemy::COMMAND_SHOOT, &AIController::testShoot }
        };
    };
} // namespace rtype
