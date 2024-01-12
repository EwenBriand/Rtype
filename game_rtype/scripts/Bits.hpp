
/*
** @file Bits.hpp
** @brief Definition of the Bits class.
** @date 2023-12-14 17:30:39.
**
*/

#pragma once

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "Observer.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the Bits.
 * It will depend on a controller, to enable controlling the Bits
 * from the network, the local player, or the ai.
 *
 */
serialize class Bits : public AUserComponent {
public:
    GENERATE_METADATA(Bits)
    Bits() = default;
    ~Bits() = default;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void SetID(int id);
    void SetClientCreator(bool creator);

private:
    /**
     * @brief broadcast the new position to all clients
     *
     */
    void broadcastPosition();

    /**
     * @brief Sends the death message to all clients
     *
     */
    void broadcastDeath();

    /**
     * @brief Shoot a laser TCEMORT
     *
     */
    void shootTcemort();

    /**
     * @brief Shoot a rocket
     *
     */
    void shootRocket();

    serialize float _speed = 60.0f;

    RigidBody2D* _rb = nullptr;
    Collider2D* _collider = nullptr;
    CoreTransform* _core = nullptr;

    int _entity;
    eng::Observer _observer;
    int _id = -1;
    bool _first = true;

    int _clientCreator = -1;
    bool _attached = false;
    int _shipID = -1;
    int _prevX = 0;
    int _prevY = 0;
    float _x = 0;
    float _y = 0;

    eng::Timer _broadcastTimer;
    eng::Timer _shootTimer;
};
