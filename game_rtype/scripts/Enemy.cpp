
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
    _broadcastTimer.Start();
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
    _rb->SetVelocity({ -_speed, 0 });

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("Enemy");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 13, "Player laser ") == 0) {
                this->_health -= 1;
                _textField->SetText((std::to_string(_health) + " HP"));
                checkDeath();
            } else if (tag.compare(0, 21, "Player LaserTcemort n") == 0) {
                this->_health = 0;
                _textField->SetText((std::to_string(_health) + " HP"));
                checkDeath();
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });

    _directivesBonus = [this](int bonusNum) {
        try {
            if (bonusNum == -1)
                return;
            std::cout << "bonus " << bonusNum << std::endl;
            std::vector<std::string> prefabNames = { "Heal", "Tcemort", "X2", "X3" };
            unsigned int e = SYS.GetResourceManager().LoadPrefab(prefabNames[bonusNum]);
            auto& enemy = SYS.GetComponent<CoreTransform>(e);
            enemy.x = _core->x;
            enemy.y = _core->y;

            serv::bytes args(std::vector<int>({ bonusNum,
                static_cast<int>(enemy.x),
                static_cast<int>(enemy.y) }));
            auto instruction = serv::Instruction(eng::RType::I_BONUS_SPAWN, 0, args);
            eng::Engine::GetEngine()->GetServer().Broadcast(instruction);

        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    };
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

    // if (not eng::Engine::GetEngine()->IsServer())
    //     return;

    _controller->PollDirectives();
    applyDirectives();
    // broadcastPosition();
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Enemy::SetID(int id)
{
    _id = id;
}

void Enemy::SetBonusOnDeath(int bonus)
{
    _bonusOnDeath = bonus;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Enemy::broadcastPosition()
{
    if (eng::Engine::GetEngine()->IsClient() or _broadcastTimer.GetElapsedTime() < 0.5f) {
        return;
    }
    _broadcastTimer.Restart();
    try {
        serv::bytes args(std::vector<int>({ _id,
            static_cast<int>(_core->x),
            static_cast<int>(_core->y) }));
        auto instruction = serv::Instruction(eng::RType::I_ENEMY_MOVES, 0, args);
        std::cout << "broadcast position " << _id << " at " << _core->x << ", " << _core->y << std::endl;
        eng::Engine::GetEngine()->GetServer().Broadcast(instruction.ToBytes() + serv::SEPARATOR);
    } catch (const std::exception& e) {
        std::cerr << "AIController::broadcastPosition(): " << e.what() << std::endl;
    }
}

void Enemy::broadcastVelocity()
{
    if (eng::Engine::GetEngine()->IsClient())
        return;

    _broadcastTimer.Restart();
    try {
        auto rb = _rb->GetVelocity();
        serv::bytes args(std::vector<int>({ _id,
            static_cast<int>(rb.x),
            static_cast<int>(rb.y) }));
        auto instruction = serv::Instruction(eng::RType::I_ENEMY_VELOCITY, 0, args);
        std::cout << "broadcast velocity " << _id << " at " << rb.x << ", " << rb.y << std::endl;
        eng::Engine::GetEngine()->GetServer().Broadcast(instruction.ToBytes() + serv::SEPARATOR);
    } catch (const std::exception& e) {
        std::cerr << "AIController::broadcastPosition(): " << e.what() << std::endl;
    }
}

void Enemy::applyDirectives()
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;

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
            _directivesBonus(_bonusOnDeath);
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
    if (_first) {
        _first = false;
        std::cout << _id << std::endl;
        _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
        // broadcastVelocity();
    }
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
