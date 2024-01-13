
/*
** @file Bits_ic.cpp
** @brief Implementation of the Bits_ic class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Bits_ic.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include <memory>

MANAGED_RESOURCE(Bits_ic)

// ===========================================================================================================
// Component
// ===========================================================================================================

void Bits_ic::OnAddComponent(int entityID)
{
    _broadcastTimer.Start();
    _entity = entityID;
}

void Bits_ic::Start()
{
    std::cout << "Bits_ic::Start()" << std::endl;
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _core = &SYS.SafeGet<CoreTransform>(_entity);

    _rb->SetVelocity({ -_speed, 0 });

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("Bits_ic");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 6, "player") == 0) {
                SYS.GetComponent<Collider2D>(_entity).SetDestroyMe(true);
                SYS.UnregisterEntity(_entity);
                std::cout << "Bits_ic::OnCollisionEnter(): " << tag << std::endl;
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
}

void Bits_ic::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        if ((_core->x < -100 or _core->x > 1920 + 100 or _core->y < -100 or _core->y > 1080 + 100)) {
            SYS.UnregisterEntity(_entity);
            std::cout << "Bits_ic::Update(): out of bounds" << std::endl;
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

void Bits_ic::SetID(int id)
{
    _id = id;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Bits_ic::broadcastPosition()
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

void Bits_ic::broadcastDeath()
{
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", eng::Engine::GetEngine()->GetGlobal<int>("killCount") + 1);
    serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id, eng::Engine::GetEngine()->GetGlobal<int>("killCount") })));
    eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
}

// ===========================================================================================================
// Directives
// ===========================================================================================================
