
/*
** @file Collider2D.hpp
** @brief Definition of the Collider2D class.
** @date 2023-12-11 14:15:41.
**
*/

#pragma once

#include "../Component.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"
#include <iostream>

/**
 * @brief A Polygon collider class for 2D entities.
 *
 */
serialize class Collider2D : public ecs::Component<Collider2D> {
public:
    GENERATE_METADATA(Collider2D)
    Collider2D() = default;
    ~Collider2D() = default;

    void Update(int entityID);
    void OnAddComponent(int entityID);
    void OnLoad();

    int GetEntityID() const;

    /**
     * @brief Adds a set of vertices to the collider.
     */
    void AddVertices(std::vector<float> vertices);

    /**
     * @brief Returns the list of edges of the collider.
     *
     */
    const std::vector<graph::vec2f>& GetEdges() const noexcept;

    /**
     * @brief Returns the list of normals of the collider.
     *
     */
    const std::vector<graph::vec2f>& GetNormals() const noexcept;

    /**
     * @brief Returns the list of vertices of the collider.
     *
     */
    const std::vector<float>& GetVertices() const noexcept;

    /**
     * @brief returns the vertices in a mutable way. (reference)
     *
     */
    std::vector<float>& GetVerticesMutable() noexcept;

    /**
     * @brief Returns true if the collider has a collision with another collider.
     * This gets reset every frame.
     *
     */
    bool IsColliding() const noexcept;

    /**
     * @brief Draws the edges of the collider if the editor mode is set
     *
     */
    void DebugDraw() const;

    /**
     * @brief Sets the tag of the collider.
     *
     */
    void SetTag(std::string tag) noexcept;

    /**
     * @brief Returns the tag of the collider.
     *
     */
    std::string GetTag() const noexcept;

    /**
     * @brief Sets the layer of the collider.
     *
     */
    void SetLayer(std::string layer) noexcept;

    /**
     * @brief Returns the layer of the collider.
     *
     */
    std::string GetLayer() const noexcept;

    /**
     * @brief Ignore this layer when checking for collisions.
     *
     */
    void IgnoreLayer(std::string layer) noexcept;

    /**
     * @brief Returns the list of layers the collider is ignoring.
     *
     */
    std::vector<std::string> GetIgnoredLayers() const noexcept;

    /**
     * @brief Sets the callback that gets called when the entity enters a collision.
     *
     */
    void SetOnCollisionEnter(std::function<void(int, int)> callback);

    /**
     * @brief Sets the callback that gets called when the entity exits a collision.
     *
     */
    void SetOnCollisionExit(std::function<void(int)> callback);

    void SetDestroyMe(bool destroyMe);

    /**
     * @brief Returns the vertices update with the position from the collider
     *
     */
    std::vector<float> GetVerticesWithPosition() const noexcept;

private:
    /**
     * @brief Returns true if the axis is separating the two colliders.
     *
     * @param axis
     * @param collider
     * @return true
     * @return false
     */
    bool isAxisSeparating(const graph::vec2f& axis, const Collider2D& a, const Collider2D& b) const noexcept;

    /**
     * @brief Returns the bounds of the projection of the polygon on the axis.
     *
     * @param axis
     * @param collider
     * @return std::pair<float, float>
     */
    std::pair<float, float> projectPolygon(const graph::vec2f& axis, const Collider2D& collider) const noexcept;

    /**
     * @brief Recalculates the edges and normals of the collider from the vertices.
     *
     */
    void recalculateEdgesAndNormals();

    /**
     * @brief Calls checkCollision on all colliders in the scene.
     *
     */
    void checkCollisions();

    /**
     * @brief Checks for a collision with the given collider using the SAT algorithm.
     *
     */
    void checkCollision(Collider2D& collider);

    /**
     * @brief The id of the entity possessing this component.
     *
     */
    int _entityID;

    bool _destroyMe = false;

    /**
     * @brief If this is set, the component will not have any influence on
     * the movement and position of the entity.
     *
     */
    bool _isTrigger = false;

    /**
     * @brief This callback gets called when the entity enters a collision, if
     * it is not null.
     *
     */
    std::function<void(int, int)> _onCollisionEnter = nullptr;

    /**
     * @brief This callback gets called when the entity exits in a collision, if
     * it is not null.
     *
     */
    std::function<void(int)> _onCollisionExit = nullptr;

    /**
     * @brief Set to true if the entity is colliding with another entity.
     * This gets reset every frame.
     *
     */
    bool _isColliding = false;

    /**
     * @brief Set to true if the entity was colliding with another entity
     * last frame.
     *
     */
    bool _wasColliding = false;

    /**
     * @brief The tag of the collider.
     *
     */
    serialize std::string _tag = "";

    /**
     * @brief The tag of the entity the collider is colliding with.
     *
     */
    std::string _collidedTag = "";

    /**
     * @brief The layer of the collider.
     *
     */
    serialize std::string _layer = "";

    /**
     * @brief The layers the collider should ignore.
     *
     */
    std::vector<std::string> _ignoreLayers = {};

    /**
     * @brief The entities the collider is colliding with.
     *
     */
    std::vector<int> _collidedEntities = {};

    /**
     * @brief The list of vertex of the collider.
     * {x1, y1, x2, y2, ...}
     *
     */
    serialize std::vector<float> _vertices = {};

    /**
     * @brief The list of edges of the collider. This is automatically
     * generated from the vertices.
     *
     */
    std::vector<graph::vec2f> _edges = {};

    /**
     * @brief The list of normals of the collider. This is automatically
     * generated from the edges.
     *
     */
    std::vector<graph::vec2f> _normals = {};
};
