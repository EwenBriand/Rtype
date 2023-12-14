
/*
** @file Ship.hpp
** @brief Definition of the Ship class.
** @date 2023-12-14 17:30:39.
**
*/

#pragma once

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IController.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the ship.
 * It will depend on a controller, to enable controlling the ship
 * from the network, the local player, or the ai.
 *
 */
serialize class Ship : public AUserComponent {
public:
    GENERATE_METADATA(Ship)
    Ship() = default;
    ~Ship() = default;

    static const std::string COMMAND_UP;
    static const std::string COMMAND_DOWN;
    static const std::string COMMAND_LEFT;
    static const std::string COMMAND_RIGHT;
    static const std::string COMMAND_SHOOT;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    /**
     * @brief Set the controller for the ship. the ship will be unresponsive
     * until a controller is set.
     *
     * @param controller
     */
    void SetController(std::shared_ptr<ecs::IController> controller);

private:
    /**
     * @brief Applies the various directives to the ship.
     *
     */
    void applyDirectives();

    serialize float _speed = 0.1f;
    serialize int _damage = 1;
    serialize int _health = 10;

    std::shared_ptr<ecs::IController> _controller;
    RigidBody2D* _rb = nullptr;
    int _entity;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void shoot();

    std::map<std::string, void (Ship::*)()> _actions = {
        { COMMAND_UP, &Ship::moveUp },
        { COMMAND_DOWN, &Ship::moveDown },
        { COMMAND_LEFT, &Ship::moveLeft },
        { COMMAND_RIGHT, &Ship::moveRight },
        { COMMAND_SHOOT, &Ship::shoot },
    };
};
