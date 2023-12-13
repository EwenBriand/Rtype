
/*
** @file Collider2D.cpp
** @brief Implementation of the Collider2D class.
** @date 2023-12-11 14:15:41.
**
*/

#include "Collider2D.hpp"
#include "CLI.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "RigidBody2D.hpp"
#include "raylib.h"

// ===============================================================
// public methods
// ===============================================================

void Collider2D::OnLoad()
{
    eng::Engine::GetEngine()->pushPipeline([]() {
        SYS.ForEach<Collider2D>([](Collider2D& collider) {
            collider.recalculateEdgesAndNormals();
        });
    },
        -600);

    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR)) {
        auto& cli = SYS.GetComponent<CLI>(SYS.GetSystemHolder());

        cli.RegisterCustomCommand(
            "col.adv", [this](CLI& c, std::vector<std::string> args) {
                auto size = args.size() - 1;
                Collider2D* collider = nullptr;

                if (size % 2 != 0) {
                    throw std::runtime_error("Invalid vertices count");
                }
                std::vector<float> vertices;
                for (size_t i = 0; i < size; i += 2) {
                    vertices.push_back(std::stof(args[i]));
                    vertices.push_back(std::stof(args[i + 1]));
                }
                if (c.GetContext() == SYS.GetSystemHolder())
                    throw std::runtime_error("system cannot have a collider");
                try {
                    collider = &SYS.GetComponent<Collider2D>(c.GetContext());
                } catch (std::exception& e) {
                    collider = &SYS.AddComponent<Collider2D>(c.GetContext());
                }
                collider->AddVertices(vertices);
                CONSOLE::info << "OK" << std::endl;
            },
            "Adds a set of vertices to the collider.");

        cli.RegisterCustomCommand(
            "col.rmv", [this](CLI& c, std::vector<std::string> args) {
                if (c.GetContext() == SYS.GetSystemHolder())
                    throw std::runtime_error("system cannot have a collider");
                try {
                    auto& collider = SYS.GetComponent<Collider2D>(c.GetContext());
                    auto& vertices = collider.GetVerticesMutable();
                    if (vertices.size() < 2) {
                        throw std::runtime_error("Cannot remove vertices from a collider with zero vertices");
                    }
                    vertices.pop_back();
                    vertices.pop_back();
                } catch (std::exception& e) {
                    throw std::runtime_error("Selected entity has no collider");
                }
            },
            "Removes the last vertices (x, y) from the collider of the selected entity.");
    }
}

void Collider2D::Update(int entityID)
{
    checkCollisions();
    DebugDraw();
}

void Collider2D::OnAddComponent(int entityID)
{
    _entityID = entityID;
}

int Collider2D::GetEntityID() const
{
    return _entityID;
}

std::vector<float>& Collider2D::GetVerticesMutable() noexcept
{
    return _vertices;
}

void Collider2D::AddVertices(std::vector<float> vertices)
{
    if (vertices.size() % 2 != 0) {
        throw std::runtime_error("Invalid vertices count");
    }
    for (auto& vertex : vertices) {
        _vertices.push_back(vertex);
    }
    recalculateEdgesAndNormals();
}

const std::vector<graph::vec2f>& Collider2D::GetNormals() const noexcept
{
    return _normals;
}

const std::vector<graph::vec2f>& Collider2D::GetEdges() const noexcept
{
    return _edges;
}

const std::vector<float>& Collider2D::GetVertices() const noexcept
{
    return _vertices;
}

bool Collider2D::IsColliding() const noexcept
{
    return _isColliding;
}

std::vector<float> Collider2D::GetVerticesWithPosition() const noexcept
{
    std::vector<float> vertices;
    try {
        auto transform = SYS.GetComponent<CoreTransform>(_entityID);
        for (int i = 0; i < _vertices.size(); i += 2) {
            vertices.push_back(_vertices[i] + transform.x);
            vertices.push_back(_vertices[i + 1] + transform.y);
        }
    } catch (std::exception& e) {
        // no transform, no position update
    }
    return vertices;
}

void Collider2D::DebugDraw() const
{
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR)) {
        auto vertices = _vertices;
        try {
            auto transform = SYS.GetComponent<CoreTransform>(_entityID);
            for (int i = 0; i < vertices.size(); i += 2) {
                vertices[i] += transform.x;
                vertices[i + 1] += transform.y;
            }
        } catch (std::exception& e) {
            // no transform, no position update
        }
        for (size_t i = 0; i < _vertices.size(); i += 2) {
            DrawLineEx({ vertices[i], vertices[i + 1] }, { vertices[(i + 2) % vertices.size()], vertices[(i + 3) % vertices.size()] }, 2, RED);
        }
    }
}

void Collider2D::SetTag(std::string tag) noexcept
{
    _tag = tag;
}

std::string Collider2D::GetTag() const noexcept
{
    return _tag;
}

void Collider2D::SetLayer(std::string layer) noexcept
{
    _layer = layer;
}

std::string Collider2D::GetLayer() const noexcept
{
    return _layer;
}

void Collider2D::SetOnCollisionEnter(std::function<void(int, int)> callback)
{
    _onCollisionEnter = callback;
}

void Collider2D::SetOnCollisionExit(std::function<void(int)> callback)
{
    _onCollisionExit = callback;
}

void Collider2D::IgnoreLayer(std::string layer) noexcept
{
    _ignoreLayers.push_back(layer);
}

std::vector<std::string> Collider2D::GetIgnoredLayers() const noexcept
{
    return _ignoreLayers;
}

// ===============================================================
// private methods
// ===============================================================

void Collider2D::recalculateEdgesAndNormals()
{
    _edges.clear();
    _normals.clear();
    std::vector<graph::vec2f> vertices;
    for (size_t i = 0; i < _vertices.size(); i += 2) {
        vertices.push_back({ _vertices[i], _vertices[i + 1] });
    }
    try {
        auto transform = SYS.GetComponent<CoreTransform>(_entityID);
        for (auto& vertex : vertices) {
            vertex.x += transform.x;
            vertex.y += transform.y;
        }
    } catch (std::exception& e) {
        // no transform, no position update
    }
    for (int i = 0; i < vertices.size(); i++) {
        _edges.push_back({ vertices[i].x - vertices[(i + 1) % vertices.size()].x, vertices[i].y - vertices[(i + 1) % vertices.size()].y });
        _normals.push_back({ _edges[i].y, -_edges[i].x });
    }
}

void Collider2D::checkCollisions()
{
    _wasColliding = _isColliding;
    SYS.ForEach<Collider2D>([this](Collider2D collider) {
        if (collider.GetEntityID() == _entityID)
            return; // doesn't check collision with self
        checkCollision(collider);
    });
    if (!_isColliding && _wasColliding && _onCollisionExit) {
        _onCollisionExit(_entityID);
    }
}

// col.adv 0 0 0 100 100 100 100 0

void Collider2D::checkCollision(Collider2D& other)
{
    other.recalculateEdgesAndNormals();
    const auto& axes1 = this->GetNormals();
    const auto& axes2 = other.GetNormals();

    for (const auto& axis : axes1)
        if (isAxisSeparating(axis, *this, other)) {
            return;
        }
    for (const auto& axis : axes2)
        if (isAxisSeparating(axis, *this, other)) {
            return;
        }
    _isColliding = true;
    if (_onCollisionEnter) {
        _onCollisionEnter(_entityID, other.GetEntityID());
    }
    try {
        SYS.GetComponent<RigidBody2D>(_entityID).CollideWith(other.GetEntityID());
    } catch (std::exception& e) {
        // no rigidbody, no collision
    }
}

bool Collider2D::isAxisSeparating(const graph::vec2f& axis, const Collider2D& a, const Collider2D& b) const noexcept
{
    auto [minA, maxA] = projectPolygon(axis, a);
    auto [minB, maxB] = projectPolygon(axis, b);

    if (maxA < minB || maxB < minA) {
        return true;
    }
    return false;
}

std::pair<float, float> Collider2D::projectPolygon(const graph::vec2f& axis, const Collider2D& collider) const noexcept
{
    std::vector<float> vertices = collider.GetVerticesWithPosition();
    float min = axis.x * vertices[0] + axis.y * vertices[1];
    float max = min;

    for (int i = 2; i < vertices.size(); i += 2) {
        float p = axis.x * vertices[i] + axis.y * vertices[i + 1];
        if (p < min) {
            min = p;
        } else if (p > max) {
            max = p;
        }
    }
    return { min, max };
}
