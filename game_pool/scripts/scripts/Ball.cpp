/*
** EPITECH PROJECT, 2023
** safe
** File description:
** Ball.hpp
*/

#include "Ball.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(Ball)

Ball::~Ball()
{
}

void Ball::Update(int entityID)
{
    if (not _isSetup) {
        setupCollider();
    }
    bounce();
    checkMapBoundariesBounce();
}

void Ball::OnAddComponent(int entityID)
{
    _entityID = entityID;
    setupCollider();
}

void Ball::Start()
{
}

void Ball::OnLoad()
{
}

// ===============================================
// Public
// ===============================================

// ===============================================
// Private
// ===============================================

void Ball::bounce()
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        auto& rigidBody = SYS.GetComponent<RigidBody3D>(_entityID);

        if (transform.y < 0) {
            while (transform.y < 0) {
                transform.y += 0.1;
            }
            auto velocity = rigidBody.GetVelocity();
            if (velocity.y < 0) {
                rigidBody.SetVelocity({ velocity.x, -velocity.y * 0.5f, velocity.z });
            }
        }
    } catch (const std::exception& e) {
        CONSOLE::err << e.what() << std::endl;
    }
}

void Ball::setupCollider()
{
    try {
        auto& collider = SYS.GetComponent<SphereCollider3D>(_entityID);

        collider.SetOnCollisionCallback([this](int thisE, int otherE) {
            auto& tag = SYS.GetComponent<Tag>(otherE);
            std::cout << "Collision with " << tag.Get() << std::endl;
            if (tag.Get() != "car") {
                return;
            }
            doCollisionPhysics(thisE, otherE);
        });
        _isSetup = true;
    } catch (const std::exception& e) {
        CONSOLE::err << e.what() << std::endl;
    }
}

void Ball::doCollisionPhysics(int thisE, int otherE)
{
    auto& otherRigidBody = SYS.GetComponent<RigidBody3D>(otherE);
    auto& thisRigidBody = SYS.GetComponent<RigidBody3D>(thisE);
    auto& otherTransform = SYS.GetComponent<CoreTransform>(otherE);
    auto& thisTransform = SYS.GetComponent<CoreTransform>(thisE);
    graph::vec3f otherRelativeDirection = otherTransform.GetWorldPosition() - thisTransform.GetWorldPosition();
    auto thisNewVelocity = otherRelativeDirection.normalized() * otherRigidBody.GetVelocity().magnitude() * 5.0f;
    thisNewVelocity.y = otherRelativeDirection.y / 2.0f;
    std::cout << "new velocity: " << thisNewVelocity.x << " " << thisNewVelocity.y << " " << thisNewVelocity.z << std::endl;

    if (thisNewVelocity.y < 0) {
        thisNewVelocity.y *= -1;
    }
    thisRigidBody.SetVelocity(thisNewVelocity);
}

void Ball::checkMapBoundariesBounce()
{

    try {
        int mapSize = eng::Engine::GetEngine()->GetGlobal<int>("mapSize");
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);

        if (transform.x > mapSize) {
            transform.x = mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_entityID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_entityID).SetVelocity({ -velocity.x * 0.9f, velocity.y, velocity.z });
        }

        if (transform.x < -mapSize) {
            transform.x = -mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_entityID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_entityID).SetVelocity({ -velocity.x * 0.9f, velocity.y, velocity.z });
        }

        if (transform.z > mapSize) {
            transform.z = mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_entityID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_entityID).SetVelocity({ velocity.x, velocity.y, -velocity.z * 0.9f });
        }

        if (transform.z < -mapSize) {
            transform.z = -mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_entityID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_entityID).SetVelocity({ velocity.x, velocity.y, -velocity.z * 0.9f });
        }
    } catch (const std::exception& e) {
        CONSOLE::err << e.what() << std::endl;
    }
}