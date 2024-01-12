
/*
** @file Force.cpp
** @brief Implementation of the Force class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Force.hpp"
#include "../GameRtype.hpp"
#include "../RTypeDistantServer.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include <memory>

MANAGED_RESOURCE(Force)

// ===========================================================================================================
// Component
// ===========================================================================================================

void Force::OnAddComponent(int entityID)
{
    _shootTimer.Start();
    _broadcastTimer.Start();
    _entity = entityID;
}

void Force::Start()
{
    std::cout << "Force::Start()" << std::endl;
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _core = &SYS.SafeGet<CoreTransform>(_entity);

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("Force");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 6, "player") == 0) {
                if (_attached)
                    return;
                _attached = true;
                _shipID = (_entity == entityID) ? otherID : entityID;
                std::cout << "Force::OnCollisionEnter(): " << tag << std::endl;
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
}

void Force::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;

    int ent = eng::Engine::GetEngine()->GetGlobal<int>("PlayerID");
    if (_shipID == ent && _attached) {
        if (_attached && _shootTimer.GetElapsedTime() > 5.0f) {
            std::cout << "Force::Update(): shoot" << std::endl;
            shootTcemort();
            _shootTimer.Restart();
        }
        try {
            auto pos = SYS.GetInputManager().MousePosition();
            auto shipPos = SYS.GetComponent<CoreTransform>(_shipID);
            if (pos.x < shipPos.x - 150 or pos.x > shipPos.x + 150)
                _core->x = shipPos.x + ((pos.x > shipPos.x + 150) ? 150 : -150);
            else
                _core->x = pos.x;
            if (pos.y < shipPos.y - 100 or pos.y > shipPos.y + 100)
                _core->y = shipPos.y + ((pos.y > shipPos.y + 100) ? 100 : -100);
            else
                _core->y = pos.y;

            if (_prevX != _core->x or _prevY != _core->y) {
                _prevX = _core->x;
                _prevY = _core->y;
                std::vector<int> data = {
                    _shipID,
                    static_cast<int>(_core->x),
                    static_cast<int>(_core->y)
                };
                if (not eng::Engine::GetEngine()->IsServer())
                    eng::Engine::GetEngine()->GetClient().Send(serv::Instruction(eng::RType::I_FORCE_MOVES, 0, serv::bytes(data)));
            }
            _attached = eng::Engine::GetEngine()->GetGlobal<bool>("ForceAttached " + std::to_string(_shipID));
        } catch (std::exception& e) {
            std::cerr << "Force::Update(): " << e.what() << std::endl;
        }

    } else if (_shipID == ent) {
        std::cout << "Force::Update(): shipID == ent" << std::endl;
        if (_prevX != _core->x or _prevY != _core->y) {
            _prevX = _core->x;
            _prevY = _core->y;
            std::vector<int> data = {
                _shipID,
                static_cast<int>(_core->x),
                static_cast<int>(_core->y)
            };
            if (not eng::Engine::GetEngine()->IsServer())
                eng::Engine::GetEngine()->GetClient().Send(serv::Instruction(eng::RType::I_FORCE_MOVES, 0, serv::bytes(data)));
        }

        if (_shootTimer.GetElapsedTime() > 2.0f) {
            std::cout << "Force::Update(): shoot2" << std::endl;
            shootRocket();
            _shootTimer.Restart();
        }
    }

    // if (not eng::Engine::GetEngine()->IsServer())
    //     return;

    // broadcastPosition();
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Force::SetID(int id)
{
    _id = id;
}

void Force::SetClientCreator(bool clientCreator)
{
    _clientCreator = clientCreator;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Force::broadcastPosition()
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

void Force::broadcastDeath()
{
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", eng::Engine::GetEngine()->GetGlobal<int>("killCount") + 1);
    serv::Instruction instruction(eng::RType::I_ENEMY_DIES, 0, serv::bytes(std::vector<int>({ _id, eng::Engine::GetEngine()->GetGlobal<int>("killCount") })));
    eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
}

void Force::shootTcemort()
{
    auto* engine = eng::Engine::GetEngine();

    if (!engine->IsServer() && _attached) {
        try {
            int laser = SYS.GetResourceManager().LoadPrefab("LaserTcemort");
            auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
            laserTransform.x = _core->x;
            laserTransform.y = _core->y - 15;

            std::vector<int> data = {
                _id,
                static_cast<int>(laserTransform.x),
                static_cast<int>(laserTransform.y)
            };
            std::cout << "\rplayer " << _id << " shoots a TCEMORT at " << laserTransform.x << ", " << laserTransform.y << std::endl;
            eng::Engine::GetEngine()->GetClient().Send(serv::Instruction(eng::RType::I_FORCE_SHOOTS_TCEMORT, 0, serv::bytes(data)));
        } catch (const std::exception& e) {
            CONSOLE::err << "\rFailed to send shoot instruction to server." << std::endl;
        };
    }
}

void Force::shootRocket()
{
    auto* engine = eng::Engine::GetEngine();

    if (!engine->IsServer() && !_attached) {
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
            eng::Engine::GetEngine()->GetClient().Send(serv::Instruction(eng::RType::I_FORCE_SHOOTS, 0, serv::bytes(data)));
        } catch (const std::exception& e) {
            CONSOLE::err << "\rFailed to send shoot instruction to server." << std::endl;
        };
    }
}
// ===========================================================================================================
// Directives
// ===========================================================================================================
