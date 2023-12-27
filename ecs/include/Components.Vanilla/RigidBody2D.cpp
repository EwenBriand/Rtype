
/*
** @file RigidBody2D.cpp
** @brief Implementation of the RigidBody2D class.
** @date 2023-12-11 18:21:43.
**
*/

#include "RigidBody2D.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

// ===========================================================
// public methods
// ===========================================================

void RigidBody2D::OnLoad()
{
    auto engine = eng::Engine::GetEngine();
    engine->pushPipeline([engine]() { // resets the acceleration of all the RigidBody2D
        if (not engine->PlayMode())
            return;
        SYS.ForEach<RigidBody2D>([](RigidBody2D& r) {
            r._acceleration = { 0, 0 };
        });
    },
        -500);

    engine->pushPipeline([engine]() { // applies gravity to all the RigidBody2D with a mass
        if (not engine->PlayMode())
            return;
        SYS.ForEach<RigidBody2D>([](RigidBody2D& r) {
            if (r.SimulateGravity())
                r._acceleration.y += 9.81;
        });
    },
        -499);

    engine->pushPipeline([engine]() { // updates the velocity of all the RigidBody2D
        if (not engine->PlayMode())
            return;
        auto deltaTime = SYS.GetDeltaTime();
        SYS.ForEach<RigidBody2D>([deltaTime](RigidBody2D& r) {
            r._velocity.x += r._acceleration.x * deltaTime;
            r._velocity.y += r._acceleration.y * deltaTime;
        });
    },
        100);

    engine->pushPipeline([engine]() { // updates the position of all the RigidBody2D
        if (not engine->PlayMode())
            return;
        auto deltaTime = SYS.GetDeltaTime();
        SYS.ForEach<RigidBody2D>([deltaTime](RigidBody2D& r) {
            try {
                auto& transform = SYS.GetComponent<CoreTransform>(r._entityID);
                r.OverridePrevPosition({ transform.x, transform.y });
                transform.x += r._velocity.x * deltaTime;
                transform.y += r._velocity.y * deltaTime;
            } catch (std::exception& e) {
                // no transform, no position
            }
        });
    },
        101);
}

void RigidBody2D::Update(int entityID)
{
}

void RigidBody2D::OnAddComponent(int entityID)
{
    _entityID = entityID;
    try {
        _transform = &SYS.GetComponent<CoreTransform>(entityID);
    } catch (std::exception& e) {
        CONSOLE::warn << "Attaching a RigidBody2D to an entity without transform" << entityID << std::endl;
    }
}

void RigidBody2D::AddForce(graph::vec2f direction)
{
    if (_mass == 0 || not _isKinematic)
        return;
    _acceleration.x += direction.x / _mass;
    _acceleration.y += direction.y / _mass;
}

graph::vec2f RigidBody2D::GetVelocity() const
{
    return _velocity;
}

graph::vec2f& RigidBody2D::GetVelocity()
{
    return _velocity;
}

void RigidBody2D::SetVelocity(graph::vec2f velocity)
{
    _velocity = velocity;
}

graph::vec2f RigidBody2D::GetAcceleration() const
{
    return _acceleration;
}

graph::vec2f& RigidBody2D::GetAcceleration()
{
    return _acceleration;
}

void RigidBody2D::SetAcceleration(graph::vec2f acceleration)
{
    _acceleration = acceleration;
}

bool RigidBody2D::IsKinematic() const
{
    return _isKinematic;
}

void RigidBody2D::SetKinematic(bool isKinematic)
{
    _isKinematic = isKinematic;
}

bool RigidBody2D::SimulateGravity() const
{
    return _simulateGravity;
}

void RigidBody2D::SetSimulateGravity(bool simulateGravity)
{
    _simulateGravity = simulateGravity;
}

float RigidBody2D::GetMass() const
{
    return _mass;
}

void RigidBody2D::SetMass(float mass)
{
    _mass = mass;
}

graph::vec2f RigidBody2D::GetPrevPosition() const
{
    graph::vec2f returnValue;
    return returnValue;
}

void RigidBody2D::OverridePrevPosition(graph::vec2f prevPosition)
{
    _prevPosition = prevPosition;
}

void RigidBody2D::SetElasticity(float elasticity)
{
    _elasticity = elasticity;
}

float RigidBody2D::GetElasticity() const
{
    return _elasticity;
}

void RigidBody2D::CollideWith(int entityID)
{
    RigidBody2D* other = nullptr;
    try {
        other = &SYS.GetComponent<RigidBody2D>(entityID);
    } catch (std::exception& e) {
        return;
    }

    if (_mass == 0 || other->_mass == 0)
        return;

    const auto& otherTransform = SYS.GetComponent<CoreTransform>(entityID);
    graph::vec2f position = { _transform->x, _transform->y };
    graph::vec2f otherPosition = { otherTransform.x, otherTransform.y };

    graph::vec2f relativeVelocity = _velocity - other->_velocity;
    graph::vec2f collisionNormal = (position - otherPosition).normalized();
    float velAlongNormal = relativeVelocity.dot(collisionNormal);

    if (velAlongNormal > 0)
        return;
    float e = std::min(_elasticity, other->_elasticity);
    float j = -(1 + e) * velAlongNormal;
    j /= (1.0 / _mass) + (1.0 / other->_mass);

    graph::vec2f impulse = collisionNormal * j;
    _velocity -= impulse * (1.0f / _mass);
    other->_velocity += impulse * (1.0 / other->_mass);
}

// ===========================================================
// private methods
// ===========================================================
