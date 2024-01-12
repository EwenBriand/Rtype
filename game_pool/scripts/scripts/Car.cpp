/*
** EPITECH PROJECT, 2023
** safe
** File description:
** Car.cpp
*/

#include "Car.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "Components.Vanilla/RigidBody3D.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(Car)

// ===============================================
// Component
// ===============================================

void Car::Start()
{
}

void Car::OnAddComponent(int entityID)
{
    _parentID = entityID;
}

void Car::Update(int entityID)
{
    if (_controller)
        _controller->PollDirectives();
    applyDirectives();
    correctAltitude();
    correctMapBounds();
}

void Car::SetID(int id)
{
}

int Car::GetID() const
{
    return 0;
}

// ===============================================
// Public
// ===============================================

// ===============================================
// Private
// ===============================================

void Car::correctAltitude()
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_parentID);
        if (transform.y < 0) {
            transform.y = 0;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Car::applyDirectives()
{
    if (not eng::Engine::GetEngine()->PlayMode()) {
        return;
    }
    if (not _controller) {
        return;
    }
    std::vector<std::string>& directives = _controller->GetDirectives();

    _isTurning = false;
    for (auto& directive : directives) {
        if (_commands.find(directive) != _commands.end()) {
            (this->*_commands[directive])();
        }
    }

    try {
        auto& rbd = SYS.GetComponent<RigidBody3D>(_parentID);
        if (not _isTurning) {
            rbd.SetAngularAcceleration({ 0, 0, 0 });
            rbd.SetAngularVelocity({ 0, 0, 0 });
        }
        if (rbd.GetVelocity().magnitude() > _maxSpeed) {
            rbd.SetVelocity(rbd.GetVelocity().normalized() * _maxSpeed);
            return;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

// ===============================================
// Commands
// ===============================================

void Car::commandLeft()
{
    auto& rbd = SYS.GetComponent<RigidBody3D>(_parentID);
    graph::vec3f forward = SYS.GetComponent<CoreTransform>(_parentID).GetForward();
    graph::vec3f left = forward.cross({ 0, 1, 0 });
    graph::vec3f force = left * rbd.GetVelocity().magnitude() * _rotationSpeed;

    _isTurning = true;

    rbd.AddTorque({ 0, _rotationSpeed, 0 });
    rbd.AddForce(force);
}

void Car::commandRight()
{
    auto& rbd = SYS.GetComponent<RigidBody3D>(_parentID);
    graph::vec3f forward = SYS.GetComponent<CoreTransform>(_parentID).GetForward();
    graph::vec3f left = forward.cross({ 0, 1, 0 });
    graph::vec3f force = left * rbd.GetVelocity().magnitude() * _rotationSpeed;

    _isTurning = true;

    rbd.AddTorque({ 0, -_rotationSpeed, 0 });
    rbd.AddForce(force);
}

void Car::commandForward()
{
    auto& rbd = SYS.GetComponent<RigidBody3D>(_parentID);
    if (rbd.GetVelocity().magnitude() > _maxSpeed) {
        return;
    }

    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_parentID);
        graph::vec3f rotation = { transform.rotationX, transform.rotationY, transform.rotationZ };
        graph::RotationMatrix3D rotationMatrix(rotation);
        graph::vec3f force = rotationMatrix * graph::vec3f { 0.0f, 0.0f, _acceleration };

        rbd.AddForce(force);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Car::commandBrake()
{
    auto& rbd = SYS.GetComponent<RigidBody3D>(_parentID);
    if (rbd.GetVelocity().magnitude() < 0.1f) {
        rbd.SetVelocity({ 0, 0, 0 });
        return;
    }
    graph::vec3f force = rbd.GetVelocity().normalized() * -_deceleration;
    rbd.AddForce(force);
}

void Car::commandBackward()
{
    auto& rbd = SYS.GetComponent<RigidBody3D>(_parentID);
    if (rbd.GetVelocity().magnitude() > _maxSpeed) {
        return;
    }

    auto& transform = SYS.GetComponent<CoreTransform>(_parentID);
    graph::vec3f rotation = { transform.rotationX, transform.rotationY, transform.rotationZ };
    graph::RotationMatrix3D rotationMatrix(rotation);
    graph::vec3f force = rotationMatrix * graph::vec3f { 0.0f, 0.0f, -_acceleration };
    rbd.AddForce(force);
}

void Car::correctMapBounds()
{
    try {
        int mapSize = eng::Engine::GetEngine()->GetGlobal<int>("mapSize");
        auto& transform = SYS.GetComponent<CoreTransform>(_parentID);

        if (transform.x > mapSize) {
            transform.x = mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_parentID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_parentID).SetVelocity({ 0, velocity.y, velocity.z });
        }

        if (transform.x < -mapSize) {
            transform.x = -mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_parentID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_parentID).SetVelocity({ 0, velocity.y, velocity.z });
        }

        if (transform.z > mapSize) {
            transform.z = mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_parentID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_parentID).SetVelocity({ velocity.x, velocity.y, 0 });
        }

        if (transform.z < -mapSize) {
            transform.z = -mapSize;
            auto velocity = SYS.GetComponent<RigidBody3D>(_parentID).GetVelocity();
            SYS.GetComponent<RigidBody3D>(_parentID).SetVelocity({ velocity.x, velocity.y, 0 });
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}