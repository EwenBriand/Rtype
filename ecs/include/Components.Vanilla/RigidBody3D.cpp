
/*
** @file RigidBody3D.cpp
** @brief Implementation of the RigidBody3D class.
** @date 2024-01-06 18:26:29.
**
*/

#include "RigidBody3D.hpp"
#include "Engine.hpp"
#include "IGraphicalModule.hpp"
#define ENGINE eng::Engine::GetEngine()

// ==============================================================================
// COMPONENT
// ==============================================================================

float RigidBody3D::PhysicsFactor = 0.01;

void RigidBody3D::Update(int entityID)
{
}

void RigidBody3D::OnAddComponent(int entityID)
{
    _entityID = entityID;
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
    } catch (std::exception& e) {
        SYS.AddComponent<CoreTransform>(_entityID);
    }
}

void RigidBody3D::OnLoad()
{

    ENGINE->pushPipeline([this]() { // clear all forces and torques for next frame to update them
        if (not ENGINE->PlayMode())
            return;
        SYS.ForEach<RigidBody3D>([](RigidBody3D& r) {
            r.SetForces({ 0, 0, 0 });
            r.SetTorques({ 0, 0, 0 });
        });
    },
        -510);

    ENGINE->pushPipeline([this]() { // resets the acceleration of all the RigidBody3D
        if (not ENGINE->PlayMode())
            return;
        SYS.ForEach<RigidBody3D>([](RigidBody3D& r) {
            r.SetAcceleration({ 0, 0, 0 });
            r.SetAngularAcceleration({ 0, 0, 0 });
        });
    },
        -510);

    ENGINE->pushPipeline([this]() { // applies gravity to all the RigidBody3D with a mass
        if (not ENGINE->PlayMode())
            return;
        SYS.ForEach<RigidBody3D>([](RigidBody3D& r) {
            if (r.SimulateGravity()) {
                r.AddForce({ 0, PhysicsFactor * -9.81f, 0 });
            }
        });
    },
        -501);

    ENGINE->pushPipeline([this]() { // applies drag to all the RigidBody3D
        if (not ENGINE->PlayMode())
            return;
        SYS.ForEach<RigidBody3D>([](RigidBody3D& r) {
            r.AddForce(r._velocity * (-r._dragFactor));
            r.AddTorque(r._angularVelocity * (-r._dragFactor));
        });
    },
        -501);

    //  update happens here

    ENGINE->pushPipeline([this]() { // updates the acceleration of all the RigidBody3D
        if (not ENGINE->PlayMode())
            return;
        auto deltaTime = SYS.GetDeltaTime();
        SYS.ForEach<RigidBody3D>([deltaTime](RigidBody3D& r) {
            if (r._mass == 0)
                return;
            r._acceleration.x = r._forces.x / (float)r._mass;
            r._acceleration.y = r._forces.y / (float)r._mass;
            r._acceleration.z = r._forces.z / (float)r._mass;
            r._angularAcceleration.x = r._torques.x / (float)r._mass;
            r._angularAcceleration.y = r._torques.y / (float)r._mass;
            r._angularAcceleration.z = r._torques.z / (float)r._mass;
        });
    },
        100);

    ENGINE->pushPipeline([this]() { // updates the velocity of all the RigidBody3D
        if (not ENGINE->PlayMode())
            return;
        auto deltaTime = SYS.GetDeltaTime();
        SYS.ForEach<RigidBody3D>([deltaTime](RigidBody3D& r) {
            r._velocity.x += r._acceleration.x * deltaTime;
            r._velocity.y += r._acceleration.y * deltaTime;
            r._velocity.z += r._acceleration.z * deltaTime;
            r._angularVelocity.x += r._angularAcceleration.x * deltaTime;
            r._angularVelocity.y += r._angularAcceleration.y * deltaTime;
            r._angularVelocity.z += r._angularAcceleration.z * deltaTime;
        });
    },
        101);

    ENGINE->pushPipeline([this]() { // updates the position of all the RigidBody3D
        if (not ENGINE->PlayMode())
            return;
        auto deltaTime = SYS.GetDeltaTime();
        SYS.ForEach<RigidBody3D>([deltaTime](RigidBody3D& r) {
            try {
                auto& transform = SYS.GetComponent<CoreTransform>(r._entityID);
                r.SetPreviousPosition({ transform.x, transform.y, transform.z });

                if (not r._lockX)
                    transform.x += r._velocity.x * deltaTime;
                if (not r._lockY)
                    transform.y += r._velocity.y * deltaTime;
                if (not r._lockZ)
                    transform.z += r._velocity.z * deltaTime;
                transform.x += r._velocity.x * deltaTime;
                transform.y += r._velocity.y * deltaTime;
                transform.z += r._velocity.z * deltaTime;
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        });
    },
        102);

    ENGINE->pushPipeline([this]() { // updates the rotation of all the RigidBody3D
        if (not ENGINE->PlayMode())
            return;
        auto deltaTime = SYS.GetDeltaTime();
        SYS.ForEach<RigidBody3D>([deltaTime](RigidBody3D& r) {
            try {
                auto& transform = SYS.GetComponent<CoreTransform>(r._entityID);
                r.SetPreviousRotation({ transform.rotationX, transform.rotationY, transform.rotationZ });

                if (not r._lockRotX)
                    transform.rotationX += r._angularVelocity.x * deltaTime;
                if (not r._lockRotY)
                    transform.rotationY += r._angularVelocity.y * deltaTime;
                if (not r._lockRotZ)
                    transform.rotationZ += r._angularVelocity.z * deltaTime;
                transform.rotationX += r._angularVelocity.x * deltaTime;
                transform.rotationY += r._angularVelocity.y * deltaTime;
                transform.rotationZ += r._angularVelocity.z * deltaTime;
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        });
    },
        102);
}

// ==============================================================================
// GETTERS / SETTERS
// ==============================================================================

// Forces
graph::vec3f RigidBody3D::GetForces() const
{
    return _forces;
}

void RigidBody3D::AddForce(graph::vec3f force)
{
    _forces += force;
}

void RigidBody3D::SetForces(graph::vec3f forces)
{
    _forces = forces;
}

void RigidBody3D::AddTorque(graph::vec3f torque)
{
    _torques += torque;
}

graph::vec3f RigidBody3D::GetTorque() const
{
    return _torques;
}

void RigidBody3D::SetTorques(graph::vec3f torque)
{
    _torques = torque;
}

// Velocity
graph::vec3f RigidBody3D::GetVelocity() const
{
    return _velocity;
}

graph::vec3f& RigidBody3D::GetVelocity()
{
    return _velocity;
}

void RigidBody3D::SetVelocity(graph::vec3f velocity)
{
    _velocity = velocity;
}

void RigidBody3D::SetAngularVelocity(graph::vec3f)
{
    _angularVelocity = _angularVelocity;
}

// Acceleration
graph::vec3f RigidBody3D::GetAcceleration() const
{
    return _acceleration;
}

graph::vec3f& RigidBody3D::GetAcceleration()
{
    return _acceleration;
}

void RigidBody3D::SetAcceleration(graph::vec3f acceleration)
{
    _acceleration = acceleration;
}

void RigidBody3D::SetAngularAcceleration(graph::vec3f angularAcceleration)
{
    _angularAcceleration = angularAcceleration;
}

// Mass
float RigidBody3D::GetMass() const
{
    return _mass;
}

// Kinematic
bool RigidBody3D::IsKinematic() const
{
    return _isKinematic;
}

void RigidBody3D::SetKinematic(bool isKinematic)
{
    _isKinematic = isKinematic;
}

// Simulate Gravity
bool RigidBody3D::SimulateGravity() const
{
    return _simulateGravity;
}

void RigidBody3D::SetSimulateGravity(bool simulateGravity)
{
    _simulateGravity = simulateGravity;
}

// Previous Position
graph::vec3f RigidBody3D::GetPreviousPosition() const
{
    return _prevPosition;
}

void RigidBody3D::SetPreviousPosition(graph::vec3f previousPosition)
{
    _prevPosition = previousPosition;
}

// Previous Rotation
graph::vec3f RigidBody3D::GetPreviousRotation() const
{
    return _prevRotation;
}

void RigidBody3D::SetPreviousRotation(graph::vec3f previousRotation)
{
    _prevRotation = previousRotation;
}

// Elasticity
void RigidBody3D::SetElasticity(float elasticity)
{
    _elasticity = elasticity;
}

float RigidBody3D::GetElasticity() const
{
    return _elasticity;
}

// Drag

void RigidBody3D::SetDragFactor(float value)
{
    _dragFactor = value;
}

float RigidBody3D::GetDragFactor() const
{
    return _dragFactor;
}

// Constraints

void RigidBody3D::SetLockX(bool lock)
{
    _lockX = lock;
}

void RigidBody3D::SetLockY(bool lock)
{
    _lockY = lock;
}

void RigidBody3D::SetLockZ(bool lock)
{
    _lockZ = lock;
}

void RigidBody3D::SetLockRotX(bool lock)
{
    _lockRotX = lock;
}

void RigidBody3D::SetLockRotY(bool lock)
{
    _lockRotY = lock;
}

void RigidBody3D::SetLockRotZ(bool lock)
{
    _lockRotZ = lock;
}

// ==============================================================================
// PUBLIC METHODS
// ==============================================================================

void RigidBody3D::CollideWith(int entity)
{
    if (_elasticity < 0) {
        try {
            auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
            transform.x = _prevPosition.x;
            transform.y = _prevPosition.y;
            transform.z = _prevPosition.z;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    doElasticCollisionWith(entity);
}

// ==============================================================================
// PRIVATE METHODS
// ==============================================================================

void RigidBody3D::doElasticCollisionWith(int entity)
{
    // simplifying bodies to be spheres
    try {
        auto& rbd = SYS.GetComponent<RigidBody3D>(entity); // the other rigidbody
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID); // this transform
        auto& otherTransform = SYS.GetComponent<CoreTransform>(entity); // the other transform

        if (_mass == 0 || rbd._mass == 0)
            return;

        graph::vec3f thisPosition = { transform.x, transform.y, transform.z };
        graph::vec3f otherPosition = { otherTransform.x, otherTransform.y, otherTransform.z };

        graph::vec3f relativeVelocity = _velocity - rbd._velocity;
        graph::vec3f collisionNormal = (thisPosition - otherPosition).normalized();
        float velocityAlongNormal = relativeVelocity.dot(collisionNormal);

        if (velocityAlongNormal > 0) // the objects are moving away from each other
            return;
        float e = std::min(_elasticity, rbd._elasticity);
        float j = -(1 + e) * velocityAlongNormal;
        j /= (1.0 / _mass) + (1.0 / rbd._mass);

        graph::vec3f impulse = collisionNormal * j;
        _velocity += impulse / _mass;
        rbd._velocity -= impulse / rbd._mass;

    } catch (std::exception& e) {
        // no rigidbody on the onther side, no physics
    }
}
