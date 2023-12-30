
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
    _speed = 100.0f;
    _rb->SetVelocity({ _speed, _rb->GetVelocity().y });

    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 12, "Player laser") == 0) {
                this->_health -= 1;
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
        const auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        if ((transform.x < -100 or transform.x > 1920 + 100 or transform.y < -100 or transform.y > 1080 + 100)) {
            SYS.UnregisterEntity(_entity);
        }
        if (_health <= 0) {
            send_death(_entity);
        }
    } catch (std::exception& e) {
        std::cerr << "Laser::Update(): " << e.what() << std::endl;
    }
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

void Enemy::SetID(int id)
{
    _id = id;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Enemy::send_death(int entity)
{
    // send message to server to add kill count

    if (not eng::Engine::GetEngine()->IsClient()) {
        return;
    }

    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entity);
        serv::bytes args(std::vector<int>({ _entity }));
        auto instruction = serv::Instruction(eng::RType::I_ENEMY_DIES, 0, args);
        auto engine = eng::Engine::GetEngine();
        if (engine->IsClient()) {
            engine->GetClient().Send(instruction);
            std::cout << "Enemy death sent" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Enemy death::sendDeath(): " << e.what() << std::endl;
    }
    SYS.UnregisterEntity(_entity);
}

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

void Enemy::checkDeath()
{
    if (not eng::Engine::GetEngine()->IsServer())
        return;
    if (_health <= 0) {
        std::shared_ptr<eng::RType> game = std::dynamic_pointer_cast<eng::RType>(eng::Engine::GetEngine()->GetGame());
        game->GetSessionData().killCount++;
        broadcastDeath();
        try {
            SYS.GetComponent<Collider2D>(_entity).SetDestroyMe(true);
            SYS.UnregisterEntity(_entity);
        } catch (std::exception& e) {
            std::cerr << "Enemy::checkDeath(): " << e.what() << std::endl;
        }
    }
}

void Enemy::broadcastDeath()
{
    serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id })));
    eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
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