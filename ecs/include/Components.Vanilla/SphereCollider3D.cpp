
/*
** @file SphereCollider3D.cpp
** @brief Implementation of the SphereCollider3D class.
** @date 2024-01-07 19:07:49.
**
*/

#include "SphereCollider3D.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "IGraphicalModule.hpp"

void SphereCollider3D::Update(int entityID)
{
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR))
        drawDebug();
    checkCollision();
}

void SphereCollider3D::OnAddComponent(int entityID)
{
    _entityID = entityID;
}

void SphereCollider3D::SetRadius(float radius)
{
    _radius = radius;
}

void SphereCollider3D::SetOnCollisionCallback(std::function<void(int, int)> callback)
{
    _onCollision = callback;
}

int SphereCollider3D::GetEntityID() const noexcept
{
    return _entityID;
}

graph::vec3f SphereCollider3D::GetPosition() const noexcept
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        return { transform.x, transform.y, transform.z };
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return { 0, 0, 0 };
    }
}

float SphereCollider3D::GetRadius() const noexcept
{
    return _radius;
}

bool SphereCollider3D::IsColliding() const noexcept
{
    return _isColliding;
}

void SphereCollider3D::SetIsColliding(bool isColliding) noexcept
{
    _isColliding = isColliding;
}

bool SphereCollider3D::IsTrigger() const noexcept
{
    return _isTrigger;
}

void SphereCollider3D::SetIsTrigger(bool isTrigger)
{
    _isTrigger = isTrigger;
}

void SphereCollider3D::SetTag(const std::string& tag) noexcept
{
    _tag = tag;
}

std::string SphereCollider3D::GetTag() const noexcept
{
    return _tag;
}

void SphereCollider3D::checkCollision()
{
    SYS.ForEach<SphereCollider3D>([this](SphereCollider3D& collider) {
        if (collider.GetEntityID() == _entityID)
            return;
        checkCollisionWith(collider);
    });
}

void SphereCollider3D::checkCollisionWith(SphereCollider3D& collider)
{
    auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
    auto& transform2 = SYS.GetComponent<CoreTransform>(collider.GetEntityID());
    auto distance = (graph::vec3f { transform.x, transform.y, transform.z } - graph::vec3f { transform2.x, transform2.y, transform2.z }).magnitude();
    if (distance < _radius + collider.GetRadius()) {
        if (_onCollision) {
            _onCollision(_entityID, collider.GetEntityID());
        }
        _isColliding = true;
    }
}

void SphereCollider3D::drawDebug()
{
    auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
    DrawSphereWires({ transform.x, transform.y, transform.z }, _radius, 10, 10, RED);
}