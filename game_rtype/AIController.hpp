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
        bool testUp();
        bool testDown();
        bool testShoot();

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
        eng::Timer _UpTimer;
        eng::Timer _DownTimer;
        float _shootInterval = 1.5f;
        float _directivesInterval = 0.0f;
        int _status = -1;
        int _id;
        std::shared_ptr<eng::Observer> _observer;
        std::vector<std::string> _directives;
        std::map<std::string, bool (AIController::*)()> _directivesTests = {
            { Enemy::COMMAND_LEFT, &AIController::testLeft },
            { Enemy::COMMAND_SHOOT, &AIController::testShoot },
            { Enemy::COMMAND_DOWN, &AIController::testDown },
            { Enemy::COMMAND_UP, &AIController::testUp },
        };
    };
} // namespace rtype
