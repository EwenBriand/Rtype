
/*
** @file Rocket.hpp
** @brief Definition of the Rocket class.
** @date 2023-12-14 17:30:39.
**
*/

#pragma once

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the Rocket.
 * It will depend on a controller, to enable controlling the Rocket
 * from the network, the local player, or the ai.
 *
 */
serialize class Rocket : public AUserComponent {
public:
    GENERATE_METADATA(Rocket)
    Rocket() = default;
    ~Rocket() = default;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void explode();

    void setDamage(int damage);
    void setSpeed(float speed);

private:
    serialize float _speed = 1.0f;
    serialize int _damage = 1;
    bool _reverse = false;

    RigidBody2D* _rb = nullptr;
    int _entity;
};
