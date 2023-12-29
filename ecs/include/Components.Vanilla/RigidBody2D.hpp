
/*
** @file RigidBody2D.hpp
** @brief Definition of the RigidBody2D class.
** @date 2023-12-11 18:21:43.
**
*/

#pragma once

#include "../Component.hpp"
#include "CoreTransform.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"

/**
 * @brief A class to handle the physics of an object according the laws of
 * Newtonian physics.
 *
 */

serialize class RigidBody2D : public ecs::Component<RigidBody2D> {
public:
    GENERATE_METADATA(RigidBody2D)
    RigidBody2D() = default;
    ~RigidBody2D() = default;

    void OnLoad();
    void OnAddComponent(int entityID);
    void Update(int entityID);

    void AddForce(graph::vec2f direction);

    /**
     * @brief Get the Velocity of the object
     *
     * @return graph::vec2f
     */
    graph::vec2f GetVelocity() const;

    /**
     * @brief Get the Velocity of the object as a reference
     *
     * @param velocity
     */
    graph::vec2f& GetVelocity();

    /**
     * @brief Set the Velocity of the object
     *
     * @param velocity
     */
    void SetVelocity(graph::vec2f);

    /**
     * @brief Get the Acceleration of the object
     *
     * @return graph::vec2f
     */
    graph::vec2f GetAcceleration() const;

    /**
     * @brief Get the Acceleration  as a reference
     *
     * @return graph::vec2f&
     */
    graph::vec2f& GetAcceleration();

    /**
     * @brief Set the Acceleration
     *
     */
    void SetAcceleration(graph::vec2f);

    /**
     * @brief returns true if the object's velocity updates the object's position
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
     * @brief returns true if gravity impacts the object's velocity
     *
     * @return true
     * @return false
     */
    bool SimulateGravity() const;

    /**
     * @brief Set the _simulateGravity property
     *
     * @param simulateGravity
     */
    void SetSimulateGravity(bool);

    /**
     * @brief Returns the position of the object at the previous frame
     *
     * @return graph::vec2f
     */
    graph::vec2f GetPrevPosition() const;

    /**
     * @brief Sets the Prev Position object for the current frame. do no use
     * unless you know what you are doing
     *
     * @param prevPosition
     */
    void OverridePrevPosition(graph::vec2f);

    /**
     * @brief Collides with an object. If the object has a Rigidbody2D, it will
     * be affected by the collision according the laws of Newtonian physics.
     *
     * @return float
     */
    void CollideWith(int entityID);

    /**
     * @brief Sets the mass of the object
     *
     */
    void SetMass(float);

    /**
     * @brief Returns the mass of the object
     *
     * @return float
     */
    float GetMass() const;

    /**
     * @brief Sets the elasticity of the object
     *
     */
    void SetElasticity(float);

    /**
     * @brief Returns the elasticity of the object
     *
     * @return float
     */
    float GetElasticity() const;

private:
    /**
     * @brief If enabled, gravity will impact the object's velocity
     *  True by default.
     */
    serialize int _simulateGravity = false;

    /**
     * @brief If enabled, the object's velocity will update the object's
     * position
     * True by default
     *
     */

    serialize int _isKinematic = true;

    /**
     * @brief The mass of the object
     *
     */

    serialize float _mass = 1;

    /**
     * @brief Elasticity of the object.
     *
     */

    serialize float _elasticity = 1;

    /**
     * @brief The velocity of the object
     *
     */
    graph::vec2f _velocity
        = { 0, 0 };

    /**
     * @brief Acceleration of the object, gets reset every frame.
     *
     */
    graph::vec2f _acceleration = { 0, 0 };

    /**
     * @brief The position of the object at the previous frame
     *
     */
    graph::vec2f _prevPosition = { 0, 0 };

    /**
     * @brief The id of the entity this component is attached to
     *
     */
    int _entityID = -1;
    /**
     * @brief a reference to the CoreTransform component of the entity this
     * component is attached to
     *
     */
    CoreTransform* _transform = nullptr;
};
