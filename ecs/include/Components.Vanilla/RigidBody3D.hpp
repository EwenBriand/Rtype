
/*
** @file RigidBody3D.hpp
** @brief Definition of the RigidBody3D class.
** @date 2024-01-06 18:26:29.
**
*/

#pragma once

#include "../Component.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"
#include <cmath>
#include <optional>
#include <raylib.h>
#include <vector>

/**
 * @brief A class that handles the physics of an object according to the laws of
 * Newtonian physics, in 3D
 *
 */
serialize class RigidBody3D : public ecs::Component<RigidBody3D> {
public:
    static float PhysicsFactor;
    GENERATE_METADATA(RigidBody3D)
    RigidBody3D() = default;
    ~RigidBody3D() = default;

    // ==============================================================================
    // COMPONENT
    // ==============================================================================

    void Update(int entityID);

    void OnLoad();

    void OnAddComponent(int entityID);

    // ==============================================================================
    // GETTERS / SETTERS
    // ==============================================================================

    /**
     * @brief Get the Velocity of the object
     *
     * @return graph::vec3f
     */
    graph::vec3f GetVelocity() const;

    /**
     * @brief Get the Velocity of the object as a reference
     *
     * @param velocity
     */
    graph::vec3f& GetVelocity();

    /**
     * @brief Set the Velocity of the object
     *
     * @param velocity
     */
    void SetVelocity(graph::vec3f);

    /**
     * @brief Get the Acceleration of the object
     *
     * @return graph::vec3f
     */
    graph::vec3f GetAcceleration() const;

    /**
     * @brief Get the Acceleration of the object as a reference
     *
     * @param acceleration
     */
    graph::vec3f& GetAcceleration();

    /**
     * @brief Set the Acceleration of the object
     *
     * @param acceleration
     */
    void SetAcceleration(graph::vec3f);

    /**
     * @brief Sets the angular acceleration of the object
     *
     */
    void SetAngularAcceleration(graph::vec3f);

    /**
     * @brief Sets the angular velocity of the object
     *
     */
    void SetAngularVelocity(graph::vec3f);

    /**
     * @brief Get the Mass of the object
     *
     * @return float
     */
    float GetMass() const;

    /**
     * @brief Returns true if the object's velocity updates the object's position
     *
     * @return true
     * @return false
     */
    bool IsKinematic() const;

    /**
     * @brief Set the _isKinematic property
     *
     * @param isKinematic
     */
    void SetKinematic(bool);

    /**
     * @brief Get the Simulate Gravity property
     *
     * @return true
     * @return false
     */
    bool SimulateGravity() const;

    /**
     * @brief Set the Simulate Gravity property
     *
     * @param simulateGravity
     */
    void SetSimulateGravity(bool);

    /**
     * @brief Returns the position of the object at the previous frame
     *
     */
    graph::vec3f GetPreviousPosition() const;

    /**
     * @brief Set the Previous Position of the object
     *
     * @param previousPosition
     */
    void SetPreviousPosition(graph::vec3f);

    /**
     * @brief Returns the rotation of the object at the previous frame
     *
     */
    graph::vec3f GetPreviousRotation() const;

    /**
     * @brief Set the Previous Rotation of the object
     *
     * @param previousRotation
     */
    void SetPreviousRotation(graph::vec3f);

    /**
     * @brief Set the Elasticity of the object
     *
     * @param elasticity
     */
    void SetElasticity(float elasticity);

    /**
     * @brief Get the Elasticity of the object
     *
     * @return float
     */
    float GetElasticity() const;

    /**
     * @brief Adds a force on the object, aligned with the center of gravity
     *
     */
    void AddForce(graph::vec3f force);

    void AddTorque(graph::vec3f torque);

    void SetTorques(graph::vec3f torques);

    graph::vec3f GetTorque() const;

    void SetForces(graph::vec3f forces);

    graph::vec3f GetForces() const;

    void SetDragFactor(float);

    float GetDragFactor() const;

    void SetLockX(bool);
    void SetLockY(bool);
    void SetLockZ(bool);
    void SetLockRotX(bool);
    void SetLockRotY(bool);
    void SetLockRotZ(bool);

    // ==============================================================================
    // Public Methods
    // ==============================================================================

    /**
     * @brief Executes the physics of a collision.
     *
     * @param entity
     */
    void CollideWith(int entity);

private:
    void doElasticCollisionWith(int entity);

    serialize float _dragFactor = 0;
    serialize int _mass = 1;
    serialize bool _isKinematic = false;
    serialize bool _simulateGravity = true;
    serialize bool _lockX = false;
    serialize bool _lockY = false;
    serialize bool _lockZ = false;
    serialize float _lockRotX = false;
    serialize float _lockRotY = false;
    serialize float _lockRotZ = false;

    /**
     * @brief if set to a negative value, the object will not bounce an simply stop on collision.
     * Its position will be reset to its position at the previous frame.
     *
     */
    serialize float _elasticity = 1;

    graph::vec3f _velocity = { 0, 0, 0 };
    graph::vec3f _angularVelocity = { 0, 0, 0 };

    graph::vec3f _acceleration = { 0, 0, 0 };
    graph::vec3f _angularAcceleration = { 0, 0, 0 };

    graph::vec3f _prevRotation = { 0, 0, 0 };
    graph::vec3f _prevPosition = { 0, 0, 0 };

    graph::vec3f _forces;
    graph::vec3f _torques;

    int _entityID = -1;
};
