
/*
** @file Speed.hpp
** @brief Definition of the Speed class.
** @date 2023-12-14 17:30:39.
**
*/

#ifndef FDB2EC00_356A_4CFB_B767_5F0B579D70D1
#define FDB2EC00_356A_4CFB_B767_5F0B579D70D1

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IController.hpp"
#include "Observer.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the Speed.
 * It will depend on a controller, to enable controlling the Speed
 * from the network, the local player, or the ai.
 *
 */
serialize class Speed : public AUserComponent {
public:
    GENERATE_METADATA(Speed)
    Speed() = default;
    ~Speed() = default;

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

#endif /* FDB2EC00_356A_4CFB_B767_5F0B579D70D1 */
