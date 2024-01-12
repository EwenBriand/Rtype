
/*
** @file Force_ic.hpp
** @brief Definition of the Force_ic class.
** @date 2023-12-14 17:30:39.
**
*/

#ifndef B9210E2A_D116_4396_A163_AC75B9921EA0
#define B9210E2A_D116_4396_A163_AC75B9921EA0

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the Force_ic.
 * It will depend on a controller, to enable controlling the Force_ic
 * from the network, the local player, or the ai.
 *
 */
serialize class Force_ic : public AUserComponent {
public:
    GENERATE_METADATA(Force_ic)
    Force_ic() = default;
    ~Force_ic() = default;

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

#endif /* B9210E2A_D116_4396_A163_AC75B9921EA0 */
