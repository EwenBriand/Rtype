
/*
** @file SphereCollider3D.hpp
** @brief Definition of the SphereCollider3D class.
** @date 2024-01-07 19:07:49.
**
*/

#pragma once

#include "../Component.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"
#include <functional>

serialize class SphereCollider3D : public ecs::Component<SphereCollider3D> {
public:
    GENERATE_METADATA(SphereCollider3D)
    SphereCollider3D() = default;
    ~SphereCollider3D() = default;

    void Update(int entityID);
    void OnAddComponent(int entityID);

    /**
     * @brief Set the radius of the sphere
     *
     */
    void SetRadius(float radius);

    /**
     * @brief Sets the callback that will be called when the sphere collides with another collider.
     *
     */
    void SetOnCollisionCallback(std::function<void(int, int)> callback);

    /**
     * @brief Returns the entity ID of the collider.
     *
     */
    int GetEntityID() const noexcept;

    /**
     * @brief Returns the position of the collider.
     *
     */
    graph::vec3f GetPosition() const noexcept;

    /**
     * @brief Returns the radius of the sphere.
     *
     */
    float GetRadius() const noexcept;

    /**
     * @brief Returns true if the sphere is colliding with another collider.
     *
     */
    bool IsColliding() const noexcept;

    /**
     * @brief Sets the colliding flag to whatever is passed in parameter.
     *
     */
    void SetIsColliding(bool colliding) noexcept;

    /**
     * @brief Returns true if the sphere is a trigger.
     *
     */
    bool IsTrigger() const noexcept;

    /**
     * @brief Sets the sphere as a trigger or not.
     *
     */
    void SetIsTrigger(bool isTrigger);

    void SetTag(const std::string& tag) noexcept;

    std::string GetTag() const noexcept;

private:
    void checkCollision();
    void checkCollisionWith(SphereCollider3D& collider);
    void drawDebug();

    serialize float _radius = 1;
    serialize bool _isTrigger = false;
    serialize std::string _tag = "";

    bool _isColliding = false;
    bool _wasColliding = false;
    int _entityID = 0;
    std::function<void(int, int)> _onCollision = nullptr;
};
