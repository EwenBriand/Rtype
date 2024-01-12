
/*
** @file Bits.cpp
** @brief Implementation of the Bits class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Bits.hpp"
#include "../GameRtype.hpp"
#include "../RTypeDistantServer.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include <memory>
#include <random>

MANAGED_RESOURCE(Bits)

// ===========================================================================================================
// Component
// ===========================================================================================================

void Bits::OnAddComponent(int entityID)
{
    _shootTimer.Start();
    _broadcastTimer.Start();
    _entity = entityID;
}

void Bits::Start()
{
    std::cout << "Bits::Start()" << std::endl;
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _core = &SYS.SafeGet<CoreTransform>(_entity);

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("Bits");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 6, "player") == 0) {
                if (_attached)
                    return;
                _x = eng::Engine::GetEngine()->GetGlobal<int>("BitsID X " + std::to_string(_entity));
                _y = eng::Engine::GetEngine()->GetGlobal<int>("BitsID Y " + std::to_string(_entity));
                _attached = true;
                _shipID = (_entity == entityID) ? otherID : entityID;
                std::cout << "Bits::OnCollisionEnter(): " << tag << " pos is " << _x << ":" << _y << std::endl;
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
}

void Bits::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        if ((_core->x < -100 or _core->x > 1920 + 100 or _core->y < -100 or _core->y > 1080 + 100)) {
            SYS.UnregisterEntity(_entity);
            std::cout << "Bits::Update(): out of bounds" << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Laser::Update(): " << e.what() << std::endl;
    }

    if (_attached) {
        if (eng::Engine::GetEngine()->IsServer() && _shootTimer.GetElapsedTime() > 5.0f) {
            std::cout << "Bits::Update(): shoot" << std::endl;
            shootRocket();
            _shootTimer.Restart();
        }
        try {
            auto shipPos = SYS.GetComponent<CoreTransform>(_shipID);
            _core->x = shipPos.x + _x;
            _core->y = shipPos.y + _y;
        } catch (std::exception& e) {
            std::cerr << "Bits::Update(): " << e.what() << std::endl;
        }
    }

    // if (not eng::Engine::GetEngine()->IsServer())
    //     return;

    // broadcastPosition();
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Bits::SetID(int id)
{
    _id = id;
}

void Bits::SetClientCreator(bool clientCreator)
{
    _clientCreator = clientCreator;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Bits::broadcastPosition()
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

void Bits::broadcastDeath()
{
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", eng::Engine::GetEngine()->GetGlobal<int>("killCount") + 1);
    serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id, eng::Engine::GetEngine()->GetGlobal<int>("killCount") })));
    eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
}

void Bits::shootRocket()
{
    auto* engine = eng::Engine::GetEngine();

    if (engine->IsServer() && _attached) {
        try {
            int laser = SYS.GetResourceManager().LoadPrefab("Rocket");
            auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
            laserTransform.x = _core->x;
            laserTransform.y = _core->y;

            std::vector<int> data = {
                _id,
                static_cast<int>(laserTransform.x),
                static_cast<int>(laserTransform.y)
            };
            std::cout << "\rplayer " << _id << " shoots a Rocket at " << laserTransform.x << ", " << laserTransform.y << std::endl;
            eng::Engine::GetEngine()->GetServer().Broadcast(serv::Instruction(eng::RType::I_FORCE_SHOOTS, 0, serv::bytes(data)));
        } catch (const std::exception& e) {
            CONSOLE::err << "\rFailed to send shoot instruction to server." << std::endl;
        };
    }
}
// ===========================================================================================================
// Directives
// ===========================================================================================================
