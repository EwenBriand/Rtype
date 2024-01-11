/*
** @file Enemy.cpp
** @brief Implementation of the Enemy class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Block.hpp"
#include "../GameRtype.hpp"
#include "../RTypeDistantServer.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "ServerUdp.hpp"
#include <memory>

MANAGED_RESOURCE(Block)


void Block::OnAddComponent(int entityID)
{
    _timer.Start();
    _entity = entityID;

}

void Block::Start()
{
    // _rb->SetVelocity({ _rb->GetVelocity().x, -_speed });
    // go top to bottom
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _core = &SYS.SafeGet<CoreTransform>(_entity);
    _textField = &SYS.SafeGet<TextField>(_entity);
    _rb->SetVelocity({ 0, _speed });
    std::cout << "velocity:" << _rb->GetVelocity().x << " " << _rb->GetVelocity().y << std::endl;
    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("Block");
}

void Block::Update(int entityID)
{
    // if (_timer.GetElapsedTime() > 5.0f) {
    //     SYS.DestroyEntity(_entity);
    // }

    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    try {
        if ((_core->x < -100 or _core->x > 1920 + 100 or _core->y < -100 or _core->y > 1080 + 100)) {
            SYS.UnregisterEntity(_entity);
        }
    } catch (std::exception& e) {
        std::cerr << "Block::Update(): " << e.what() << std::endl;
    }

        std::cout << "UPDATE velocity:" << _rb->GetVelocity().x << " " << _rb->GetVelocity().y << std::endl;
}

void Block::SetID(int id)
{
    _id = id;
}

