/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossLaser.cpp
*/

#include "BossLaser.hpp"
#include "Components.Vanilla/Collider2D.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(BossLaser)

void BossLaser::Start()
{
    try {
        auto& collider = SYS.GetComponent<Collider2D>(_entityID);
        collider.SetOnCollisionEnter([this](int entityID, int otherID) {
            try {
                auto& collider = SYS.GetComponent<Collider2D>(_entityID);
                auto& otherCollider = SYS.GetComponent<Collider2D>(otherID);
                if (otherCollider.GetTag() == "player") {
                    collider.SetDestroyMe(true);
                    SYS.UnregisterEntity(_entityID);
                }
            } catch (std::exception& e) {
                std::cerr << "BossLaser::OnCollisionEnter(): " << e.what() << std::endl;
            }
        });
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void BossLaser::OnAddComponent(int entityID)
{
    _entityID = entityID;
}

void BossLaser::Update(int entityID)
{
    if (not eng::Engine::GetEngine()->PlayMode())
        return;
    if (destroyIfOutOfScreen())
        return;
    move();
}

// ===========================================================================================================
// GETTERS & SETTERS
// ===========================================================================================================
void BossLaser::SetDirection(graph::vec2f direction)
{
    _direction = direction.normalized();
}

// ===========================================================================================================
// PUBLIC METHODS
// ===========================================================================================================
void BossLaser::move()
{
    if (_entityID == -1)
        throw std::runtime_error("BossLaser::move: entityID not set");
    auto& transform = SYS.GetComponent<CoreTransform>(_entityID);

    transform.x += _direction.x * _speed * SYS.GetDeltaTime();
    transform.y += _direction.y * _speed * SYS.GetDeltaTime();
}

bool BossLaser::destroyIfOutOfScreen()
{
    if (_entityID == -1)
        throw std::runtime_error("BossLaser::destroyIfOutOfScreen: entityID not set");
    auto& transform = SYS.GetComponent<CoreTransform>(_entityID);

    if (transform.x < 0 || transform.x > 1920 || transform.y < 0 || transform.y > 1080) {
        SYS.UnregisterEntity(_entityID);
        return true;
    }
    return false;
}