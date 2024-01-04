
/*
** @file Enemy2.cpp
** @brief Implementation of the Enemy2 class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Enemy2.hpp"
#include "../GameRtype.hpp"
#include "../RTypeDistantServer.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include <memory>

MANAGED_RESOURCE(Enemy2)

const std::string Enemy2::COMMAND_LEFT = "left";
const std::string Enemy2::COMMAND_SHOOT = "shoot";
const std::string Enemy2::COMMAND_UP = "up";
const std::string Enemy2::COMMAND_DOWN = "down";
// ===========================================================================================================
// Component
// ===========================================================================================================

void Enemy2::OnAddComponent(int entityID)
{
    _timer.Start();
    _entity = entityID;
}

void Enemy2::Start()
{
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _core = &SYS.SafeGet<CoreTransform>(_entity);

    _textField = &SYS.SafeGet<TextField>(_entity);
    _textField->SetPosition({ -80, -80 });
    _textField->SetText((std::to_string(_health) + " HP"));

    _speed = 100.0f;
    _rb->SetVelocity({ _speed, _rb->GetVelocity().y });

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("Enemy2");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 12, "Player laser") == 0) {
                this->_health -= 1;
                _textField->SetText((std::to_string(_health) + " HP"));
                checkDeath();
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
}

void Enemy2::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        if ((_core->x < -100 or _core->x > 1920 + 100 or _core->y < -100 or _core->y > 1080 + 100)) {
            SYS.UnregisterEntity(_entity);
        }
    } catch (std::exception& e) {
        std::cerr << "Laser::Update(): " << e.what() << std::endl;
    }
    if (!_controller) {
        return;
    }

    _controller->PollDirectives();
    if (_core->y > 1080 - 100 || _core->y < 50)
        _rb->SetVelocity({ 0, (_rb->GetVelocity().y >= 0) ? -_speed : _speed });
    else
        _rb->SetVelocity({ 0, _rb->GetVelocity().y });
    applyDirectives();
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Enemy2::SetID(int id)
{
    _id = id;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Enemy2::applyDirectives()
{
    if (not eng::Engine::GetEngine()->PlayMode()) {
        return;
    }
    std::vector<std::string>& directives = _controller->GetDirectives();
    for (auto& directive : directives) {
        if (_actions.find(directive) != _actions.end()) {
            (this->*_actions[directive])();
        }
    }
}

void Enemy2::checkDeath()
{
    if (not eng::Engine::GetEngine()->IsServer())
        return;
    if (_health <= 0) {
        std::shared_ptr<eng::RType> game = std::dynamic_pointer_cast<eng::RType>(eng::Engine::GetEngine()->GetGame());
        game->GetSessionData().killCount++;
        broadcastDeath();
        try {
            _collider->SetDestroyMe(true);
            SYS.UnregisterEntity(_entity);
        } catch (std::exception& e) {
            std::cerr << "Enemy2::checkDeath(): " << e.what() << std::endl;
        }
    }
}

void Enemy2::broadcastDeath()
{
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", eng::Engine::GetEngine()->GetGlobal<int>("killCount") + 2);
    // serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id, eng::Engine::GetEngine()->GetGlobal<int>("killCount") })));
    serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id })));
    eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
}
// ===========================================================================================================
// Directives
// ===========================================================================================================

void Enemy2::shoot()
{
    broadcastShoot(_core->x, _core->y - 10);
    instantiateRedLaser(_core->x, _core->y - 10);
    broadcastShoot(_core->x, _core->y + 10);
    instantiateRedLaser(_core->x, _core->y + 10);
}

void Enemy2::moveLeft()
{
    _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
}

void Enemy2::moveUp()
{
    if (_core->y > 50)
        _rb->SetVelocity({ _rb->GetVelocity().x, _speed });
}

void Enemy2::moveDown()
{
    _rb->SetVelocity({ _rb->GetVelocity().x, _speed });
}

void Enemy2::broadcastShoot(int x, int y)
{
    if (not eng::Engine::GetEngine()->IsServer()) // only server can broadcast
        return;
    try {
        serv::bytes args(std::vector<int>({ _id,
            static_cast<int>(x),
            static_cast<int>(y) }));
        auto instruction = serv::Instruction(eng::RType::I_ENEMY_SHOOTS, 0, args);
        eng::Engine::GetEngine()->GetServer().Broadcast(instruction);

    } catch (const std::exception& e) {
        std::cerr << "AIController::broadcastShoot(): " << e.what() << std::endl;
    }
}

void Enemy2::instantiateRedLaser(int x, int y)
{
    try {
        auto laser = SYS.GetResourceManager().LoadPrefab("red-laser");
        auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = x;
        laserTransform.y = y;
    } catch (const std::exception& e) {
        std::cerr << "AIController::instantiateRedLaser(): " << e.what() << std::endl;
    }
}