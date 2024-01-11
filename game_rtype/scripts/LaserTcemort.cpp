
/*
** @file LaserTcemort.cpp
** @brief Implementation of the LaserTcemort class.
** @date 2023-12-14 17:30:39.
**
*/

#include "LaserTcemort.hpp"
#include "Components.Vanilla/Tag.hpp"
#include "ECSImpl.hpp"

MANAGED_RESOURCE(LaserTcemort)

// ===========================================================================================================
// Component
// ===========================================================================================================

void LaserTcemort::OnAddComponent(int entityID)
{
    _entity = entityID;
    _timer.Start();
}

void LaserTcemort::Start()
{
    auto& collider = SYS.SafeGet<Collider2D>(_entity);

    if (_reverse) {
        collider.SetTag("Enemy LaserTcemort n°" + std::to_string(_entity));
    } else {
        collider.SetTag("Player LaserTcemort n°" + std::to_string(_entity));
    }
}

void LaserTcemort::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        const auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        if (_timer.GetElapsedTime() > _timeOfLife) {
            SYS.UnregisterEntity(_entity);
        }
    } catch (std::exception& e) {
        std::cerr << "LaserTcemort::Update(): " << e.what() << std::endl;
    }
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void LaserTcemort::explode()
{
}

void LaserTcemort::setDamage(int damage)
{
    _damage = damage;
}

void LaserTcemort::setSpeed(float speed)
{
    _speed = speed;
}
