
/*
** @file Laser.cpp
** @brief Implementation of the Laser class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Laser.hpp"
#include "Components.Vanilla/Tag.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

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
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    auto& collider = SYS.SafeGet<Collider2D>(_entity);
    _speed = 150.0f;

    try {
        auto& tag = SYS.GetComponent<Tag>(_entity);
        if (tag.Get() == "enemy") {
            _reverse = true;
        }
    } catch (std::exception& e) {
        // no need to reverse
    }

    if (_reverse)
        _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
    else
        _rb->SetVelocity({ _speed, _rb->GetVelocity().y });
}

void Laser::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        const auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        if (transform.x < -100 or transform.x > 1920 + 100 or transform.y < -100 or transform.y > 1080 + 100) {
            SYS.UnregisterEntity(_entity);
        }
    } catch (std::exception& e) {
        std::cerr << "Laser::Update(): " << e.what() << std::endl;
    }
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
