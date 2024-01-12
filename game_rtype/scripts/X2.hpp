
/*
** @file X2.hpp
** @brief Definition of the X2 class.
** @date 2023-12-14 17:30:39.
**
*/

#pragma once

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the X2.
 * It will depend on a controller, to enable controlling the X2
 * from the network, the local player, or the ai.
 *
 */
serialize class X2 : public AUserComponent {
public:
    GENERATE_METADATA(X2)
    X2() = default;
    ~X2() = default;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void SetID(int id);

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

    serialize float _speed = 60.0f;

    RigidBody2D* _rb = nullptr;
    Collider2D* _collider = nullptr;
    CoreTransform* _core = nullptr;

    int _entity;
    int _id = -1;
    bool _first = true;

    eng::Timer _broadcastTimer;
};
