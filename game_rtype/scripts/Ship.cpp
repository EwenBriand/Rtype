
/*
** @file Ship.cpp
** @brief Implementation of the Ship class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Ship.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(Ship)

const std::string Ship::COMMAND_UP = "up";
const std::string Ship::COMMAND_DOWN = "down";
const std::string Ship::COMMAND_LEFT = "left";
const std::string Ship::COMMAND_RIGHT = "right";
const std::string Ship::COMMAND_SHOOT = "shoot";

// ===========================================================================================================
// Component
// ===========================================================================================================

void Ship::OnAddComponent(int entityID)
{
    _entity = entityID;
}

void Ship::Start()
{
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    auto& collider = SYS.SafeGet<Collider2D>(_entity);
}

void Ship::Update(int entityID)
{
    if (!_controller) {
        return;
    }
    _controller->PollDirectives();
    _rb->SetVelocity({ 0, 0 });
    applyDirectives();
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Ship::applyDirectives()
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

void Ship::moveUp()
{
    _rb->SetVelocity({ _rb->GetVelocity().x, -_speed });
}

void Ship::moveDown()
{
    _rb->SetVelocity({ _rb->GetVelocity().x, _speed });
}

void Ship::moveLeft()
{
    _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
}

void Ship::moveRight()
{
    _rb->SetVelocity({ _speed, _rb->GetVelocity().y });
}

void Ship::shoot()
{
    int laser = SYS.GetResourceManager().LoadPrefab("Laser");
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = transform.x;
        laserTransform.y = transform.y;
    } catch (std::exception& e) {
        return;
    }
}
