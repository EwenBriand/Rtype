/*
** @file Enemy2.hpp
** @brief Definition of the Enemy2 class.
** @date 2023-12-14 17:30:39.
**
*/

#pragma once

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/TextField.hpp"
#include "Components.Vanilla/UIDiv.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "Observer.hpp"
#include "metadata.hpp"

BUILD_MANUAL

/**
 * @brief This class is a script to control the Enemy2.
 * It will depend on a controller, to enable controlling the Enemy2
 * from the network, the local player, or the ai.
 *
 */
serialize class Enemy2 : public AUserComponent, public ecs::AActor {
public:
    GENERATE_METADATA(Enemy2)
    Enemy2() = default;
    ~Enemy2() = default;

    static const std::string COMMAND_LEFT;
    static const std::string COMMAND_SHOOT;
    static const std::string COMMAND_UP;
    static const std::string COMMAND_DOWN;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void SetID(int id);

private:
    /**
     * @brief Applies the various directives to the Enemy2.
     *
     */
    void applyDirectives();

    /**
     * @brief broadcast the new position to all clients
     *
     */
    void broadcastPosition();

    /**
     * @brief broadcast the new velocity to all clients
     *
     */
    void broadcastVelocity();
    
    /**
     * @brief Checks if the Enemy2 should die, and if so, kills it and
     * notifies the game and clients.
     *
     */
    void checkDeath();
    void instantiateRedLaser(int x, int y);
    void broadcastShoot(int x, int y);

    /**
     * @brief Sends the death message to all clients
     *
     */
    void broadcastDeath();

    serialize float _speed = 1.0f;
    serialize int _damage = 1;
    serialize int _health = 10;
    serialize int _delay = 2;
    eng::Timer _timer;

    RigidBody2D* _rb = nullptr;
    Collider2D* _collider = nullptr;
    CoreTransform* _core = nullptr;
    TextField* _textField = nullptr;
    int _entity;
    eng::Observer _observer;
    int _id = -1;
    bool _first = true;

    eng::Timer _broadcastTimer;

    std::map<std::string, void (Enemy2::*)()> _actions = {
        { COMMAND_LEFT, &Enemy2::moveLeft },
        { COMMAND_SHOOT, &Enemy2::shoot },
        { COMMAND_UP, &Enemy2::moveUp },
        { COMMAND_DOWN, &Enemy2::moveDown },

    };
    void shoot();
    void moveLeft();
    void moveUp();
    void moveDown();
};
