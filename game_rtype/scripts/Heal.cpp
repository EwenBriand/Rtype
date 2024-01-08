
/*
** @file Heal.cpp
** @brief Implementation of the Heal class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Heal.hpp"
#include "../GameRtype.hpp"
#include "../RTypeDistantServer.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include <memory>

MANAGED_RESOURCE(Heal)

// ===========================================================================================================
// Component
// ===========================================================================================================

void Heal::OnAddComponent(int entityID)
{
    _timer.Start();
    _broadcastTimer.Start();
    _entity = entityID;
}

void Heal::Start()
{
    std::cout << "Heal::Start()" << std::endl;
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _core = &SYS.SafeGet<CoreTransform>(_entity);

    _rb->SetVelocity({ -_speed, 0 });

    auto& sprite = SYS.SafeGet<Animator>(_entity);

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("Heal");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 6, "player") == 0) {
                SYS.GetComponent<Collider2D>(_entity).SetDestroyMe(true);
                SYS.UnregisterEntity(_entity);
                std::cout << "Heal::OnCollisionEnter(): " << tag << std::endl;
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
}

void Heal::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        if ((_core->x < -100 or _core->x > 1920 + 100 or _core->y < -100 or _core->y > 1080 + 100)) {
            SYS.UnregisterEntity(_entity);
            std::cout << "Heal::Update(): out of bounds" << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Laser::Update(): " << e.what() << std::endl;
    }

    // if (not eng::Engine::GetEngine()->IsServer())
    //     return;

    // broadcastPosition();
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Heal::SetID(int id)
{
    _id = id;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Heal::broadcastPosition()
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

void Heal::broadcastDeath()
{
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", eng::Engine::GetEngine()->GetGlobal<int>("killCount") + 1);
    serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id, eng::Engine::GetEngine()->GetGlobal<int>("killCount") })));
    eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
}

// ===========================================================================================================
// Directives
// ===========================================================================================================
