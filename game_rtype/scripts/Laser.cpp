
/*
** @file Laser.cpp
** @brief Implementation of the Laser class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Laser.hpp"
#include "ECSImpl.hpp"

MANAGED_RESOURCE(Laser)

// ===========================================================================================================
// Component
// ===========================================================================================================

void Laser::OnAddComponent(int entityID)
{
    _entity = entityID;
}

void Laser::Start()
{
    std::cout << "Laser start" << std::endl;
    _rb = &SYS.GetComponent<RigidBody2D>(_entity);
    auto& collider = SYS.SafeGet<Collider2D>(_entity);
    _speed = 100.0f;
    _rb->SetVelocity({ _speed, _rb->GetVelocity().y });
}

void Laser::Update(int entityID)
{
    // std::cout << "Laser update" << std::endl;
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Laser::explode()
{
}

void Laser::setDamage(int damage)
{
    _damage = damage;
}

void Laser::setSpeed(float speed)
{
    _speed = speed;
}
