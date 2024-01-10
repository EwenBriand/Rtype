
/*
** @file Ship.hpp
** @brief Definition of the Ship class.
** @date 2023-12-14 17:30:39.
**
*/

#pragma once

#include "Components.Vanilla/AudioSource.hpp"
#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "Observer.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the ship.
 * It will depend on a controller, to enable controlling the ship
 * from the network, the local player, or the ai.
 *
 */
serialize class Ship : public AUserComponent, public ecs::AActor {
public:
    GENERATE_METADATA(Ship)
    Ship() = default;
    ~Ship() = default;

    static const std::string COMMAND_UP;
    static const std::string COMMAND_DOWN;
    static const std::string COMMAND_LEFT;
    static const std::string COMMAND_RIGHT;
    static const std::string COMMAND_SHOOT;
    static const std::string COMMAND_SHOOT_TCEMORT;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void SetID(int id);
    int GetID() const;
    int GetNbLaser() const;
    CoreTransform* GetCore() const;

private:
    serialize float _speed = 1.0f;
    serialize int _damage = 1;
    serialize int _health = 10;

    /**
     * @brief Applies the various directives to the ship.
     *
     */
    void applyDirectives();

    /**
     * @brief Checks if the ship has been destroyed and if yes, destroys it and broadcasts the news to all clients.
     *
     */
    void checkForDeath();

    /**
     * @brief Setup all collisions lambda.
     *
     */
    void SetupCollisions();

    /**
     * @brief Send a shoot to the server.
     *
     * @param x The x position of the shoot.
     * @param y The y position of the shoot.
     */
    void SendShoot(int x, int y);

    /**
     * @brief create 1 laser
     *
     */
    void creatOneLaser();

    /**
     * @brief create 2 laser
     *
     */
    void creatTwoLaser();

    /**
     * @brief create 3 laser
     *
     */
    void creatThreeLaser();

    RigidBody2D* _rb = nullptr;
    Collider2D* _collider = nullptr;
    AudioSource* _audio = nullptr;
    CoreTransform* _core = nullptr;
    TextField* _textField = nullptr;

    int _entity;
    eng::Observer _observer;
    int _id = -1;
    int _nb_laser = 1;
    bool _tcemort = false;
    int _forceID = -1;
    bool _force_existing = false;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void shoot();
    void shootTcemort();

    std::map<std::string, void (Ship::*)()> _actions = {
        { COMMAND_UP, &Ship::moveUp },
        { COMMAND_DOWN, &Ship::moveDown },
        { COMMAND_LEFT, &Ship::moveLeft },
        { COMMAND_RIGHT, &Ship::moveRight },
        { COMMAND_SHOOT, &Ship::shoot },
        { COMMAND_SHOOT_TCEMORT, &Ship::shootTcemort },
    };
};
