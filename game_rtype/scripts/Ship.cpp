
/*
** @file Ship.cpp
** @brief Implementation of the Ship class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Ship.hpp"
#include "ECSImpl.hpp"

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
    _rb = &SYS.GetComponent<RigidBody2D>(_entity);
    auto& collider = SYS.SafeGet<Collider2D>(_entity);
}

void Ship::Update(int entityID)
{
    if (!_controller) {
        CONSOLE::warn << "Ship: Controller not set" << std::endl;
        return;
    }
    applyDirectives();
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Ship::SetController(std::shared_ptr<ecs::IController> controller)
{
    _controller = std::move(controller);
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Ship::applyDirectives()
{
    std::vector<std::string>& directives = _controller->GetDirectives();
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
}