
/*
** @file Nuke.hpp
** @brief Definition of the Nuke class.
** @date 2023-12-14 17:30:39.
**
*/

#ifndef BF6EB14F_1C02_49F3_A106_A16A60C2C17F
#define BF6EB14F_1C02_49F3_A106_A16A60C2C17F

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "Observer.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the Nuke.
 * It will depend on a controller, to enable controlling the Nuke
 * from the network, the local player, or the ai.
 *
 */
serialize class Nuke : public AUserComponent {
public:
    GENERATE_METADATA(Nuke)
    Nuke() = default;
    ~Nuke() = default;

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
    eng::Observer _observer;
    int _id = -1;
    bool _first = true;

    eng::Timer _broadcastTimer;
};

#endif /* BF6EB14F_1C02_49F3_A106_A16A60C2C17F */
