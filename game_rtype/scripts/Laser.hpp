
/*
** @file Laser.hpp
** @brief Definition of the Laser class.
** @date 2023-12-14 17:30:39.
**
*/

#ifndef E17B801E_E5BF_4E3F_9A10_E9531CD0E90B
#define E17B801E_E5BF_4E3F_9A10_E9531CD0E90B

#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "metadata.hpp"

/**
 * @brief This class is a script to control the Laser.
 * It will depend on a controller, to enable controlling the Laser
 * from the network, the local player, or the ai.
 *
 */
serialize class Laser : public AUserComponent, public ecs::AActor {
public:
    GENERATE_METADATA(Laser)
    Laser() = default;
    ~Laser() = default;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void explode();

    void setDamage(int damage);
    void setSpeed(float speed);

private:
    serialize float _speed = 1.0f;
    serialize int _damage = 1;

    RigidBody2D* _rb = nullptr;
    int _entity;
};

#endif /* E17B801E_E5BF_4E3F_9A10_E9531CD0E90B */
