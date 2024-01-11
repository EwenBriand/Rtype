
/*
** @file Rocket.cpp
** @brief Implementation of the Rocket class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Rocket.hpp"
#include "Components.Vanilla/Tag.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(Rocket)

// ===========================================================================================================
// Component
// ===========================================================================================================

void Rocket::OnAddComponent(int entityID)
{
    _entity = entityID;
}

void Rocket::Start()
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

    if (_reverse) {
        collider.SetTag("Enemy Rocket n°" + std::to_string(_entity));
        _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
        collider.SetOnCollisionEnter([this](int entityID, int otherID) {
            try {
                std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
                if (tag == "player" || tag == "Force") {
                    SYS.GetComponent<Collider2D>(_entity).SetDestroyMe(true);
                    SYS.UnregisterEntity(_entity);
                }
            } catch (std::exception& e) {
                std::cerr << "Rocket::OnCollisionEnter(): " << e.what() << std::endl;
            }
        });
    } else {
        collider.SetTag("Player Rocket n°" + std::to_string(_entity));
        _rb->SetVelocity({ _speed, _rb->GetVelocity().y });
        collider.SetOnCollisionEnter([this](int entityID, int otherID) {
            try {
                std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
                if (tag == "Enemy" || tag == "Enemy2") {
                    SYS.GetComponent<Collider2D>(_entity).SetDestroyMe(true);
                    SYS.UnregisterEntity(_entity);
                }
                if (tag == "boss") {
                    eng::Engine::GetEngine()->SetGlobal("bossHp", eng::Engine::GetEngine()->GetGlobal<int>("bossHp") - _damage);
                    SYS.GetComponent<Collider2D>(_entity).SetDestroyMe(true);
                    SYS.UnregisterEntity(_entity);
                    std::cout << "hit boss" << std::endl;
                }
            } catch (std::exception& e) {
                std::cerr << "Rocket::OnCollisionEnter: " << e.what() << std::endl;
            }
        });
    }
}

void Rocket::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        const auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        if (transform.x < -100 or transform.x > 1920 + 100 or transform.y < -100 or transform.y > 1080 + 100) {
            SYS.UnregisterEntity(_entity);
        }
    } catch (std::exception& e) {
        std::cerr << "Rocket::Update(): " << e.what() << std::endl;
    }
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Rocket::explode()
{
}

void Rocket::setDamage(int damage)
{
    _damage = damage;
}

void Rocket::setSpeed(float speed)
{
    _speed = speed;
}
