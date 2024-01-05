
/*
** @file Enemy.cpp
** @brief Implementation of the Enemy class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Enemy.hpp"
#include "../GameRtype.hpp"
#include "../RTypeDistantServer.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include <memory>

MANAGED_RESOURCE(Enemy)

const std::string Enemy::COMMAND_LEFT = "left";
const std::string Enemy::COMMAND_SHOOT = "shoot";
const std::string Enemy::COMMAND_UP = "up";
const std::string Enemy::COMMAND_DOWN = "down";
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
    _core = &SYS.SafeGet<CoreTransform>(_entity);

    _textField = &SYS.SafeGet<TextField>(_entity);
    _textField->SetPosition({ -80, -80 });
    _textField->SetText((std::to_string(_health) + " HP"));

    _speed = 100.0f;
    _rb->SetVelocity({ _speed, _rb->GetVelocity().y });

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("enemy");
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

void Enemy::Update(int entityID)
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

void Enemy::SetID(int id)
{
    _id = id;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Enemy::applyDirectives()
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

void Enemy::checkDeath()
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
            std::cerr << "Enemy::checkDeath(): " << e.what() << std::endl;
        }
    }
}

void Enemy::broadcastDeath()
{
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", eng::Engine::GetEngine()->GetGlobal<int>("killCount") + 1);
    serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id, eng::Engine::GetEngine()->GetGlobal<int>("killCount") })));
    // serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id })));
    eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
}
// ===========================================================================================================
// Directives
// ===========================================================================================================

void Enemy::shoot()
{
    broadcastShoot();
    instantiateRedLaser();
}

void Enemy::moveLeft()
{
    _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
}

void Enemy::moveUp()
{
}

void Enemy::moveDown()
{
}

void Enemy::broadcastShoot()
{
    if (not eng::Engine::GetEngine()->IsServer()) // only server can broadcast
        return;
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        serv::bytes args(std::vector<int>({ _id,
            static_cast<int>(transform.x),
            static_cast<int>(transform.y) }));
        auto instruction = serv::Instruction(eng::RType::I_ENEMY_SHOOTS, 0, args);
        eng::Engine::GetEngine()->GetServer().Broadcast(instruction);

    } catch (const std::exception& e) {
        std::cerr << "AIController::broadcastShoot(): " << e.what() << std::endl;
    }
}

void Enemy::instantiateRedLaser()
{
    try {
        auto laser = SYS.GetResourceManager().LoadPrefab("red-laser");
        auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = transform.x;
        laserTransform.y = transform.y;
    } catch (const std::exception& e) {
        std::cerr << "AIController::instantiateRedLaser(): " << e.what() << std::endl;
    }
}
