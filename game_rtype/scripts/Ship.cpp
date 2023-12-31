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
    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("player");
    std::cout << "player id " << _entity << std::endl;

    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::cout << "Ship::Start(): " << entityID << " " << otherID << std::endl;
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            std::cout << "after tag" << std::endl;
            if (tag.compare(0, 11, "Enemy laser") == 0) {
                std::cout << "before test" << std::endl;
                this->_health -= 1;
                std::cout << "after test " << _entity << std::endl;
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
}

void Ship::Update(int entityID)
{
    if (!_controller) {
        return;
    }
    _controller->PollDirectives();
    _rb->SetVelocity({ 0, 0 });
    applyDirectives();
    if (_health <= 0) {
        SYS.UnregisterEntity(_entity);
    }
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
    std::cout << "Ship::shoot()" << std::endl;
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
