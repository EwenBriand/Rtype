
/*
** @file Enemy.cpp
** @brief Implementation of the Enemy class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Enemy.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(Enemy)

const std::string Enemy::COMMAND_LEFT = "left";
const std::string Enemy::COMMAND_SHOOT = "shoot";
// ===========================================================================================================
// Component
// ===========================================================================================================

void Enemy::OnAddComponent(int entityID)
{
    _timer.Start();
    _entity = entityID;
}

void Enemy::Start()
{
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("enemy");
    std::cout << "enemy id " << _entity << std::endl;
    _speed = 100.0f;
    _rb->SetVelocity({ _speed, _rb->GetVelocity().y });

    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            // std::cout << "Ship::Start(): " << entityID << " " << otherID << std::endl;
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            // std::cout << "after tag" << std::endl;
            if (tag.compare(0, 12, "Player laser") == 0) {
                // std::cout << "before test" << std::endl;
                this->_health -= 1;
                // std::cout << "after test " << _entity << std::endl;
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
}

void Enemy::Update(int entityID)
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
    // if (_timer.GetElapsedTime() > _delay) {
    //     shoot();
    //     _timer.Restart();
    // }
    if (!_controller) {
        return;
    }
    _controller->PollDirectives();
    _rb->SetVelocity({ 0, 0 });
    applyDirectives();

    std::cout << "Enemy::Update(): " << _rb->GetPrevPosition().x << std::endl;
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Enemy::applyDirectives()
{
    if (not eng::Engine::GetEngine()->PlayMode()) {
        return;
    }
    std::vector<std::string>& directives = _controller->GetDirectives();
    _rb->SetVelocity({ 0, 0 });
    for (auto& directive : directives) {
        if (_actions.find(directive) != _actions.end()) {
            (this->*_actions[directive])();
        }
    }
}
// ===========================================================================================================
// Directives
// ===========================================================================================================

void Enemy::shoot()
{
    int laser = SYS.GetResourceManager().LoadPrefab("Laser-Enemy");
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = transform.x;
        laserTransform.y = transform.y;
    } catch (std::exception& e) {
        return;
    }
}

void Enemy::moveLeft()
{
    _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
}
