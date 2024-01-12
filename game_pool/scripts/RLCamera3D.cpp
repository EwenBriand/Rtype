/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** RLCamera3D.cpp.
*/

#include "RLCamera3D.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include <cmath>

MANAGED_RESOURCE(RLCamera3D)

RLCamera3D::~RLCamera3D()
{
    std::cout << "destroy camera" << std::endl;
    if (m_mode3DEnabled)
        EndMode3D();
    std::cout << "destroyed camera" << std::endl;
}

void RLCamera3D::Update(int entityID)
{
    updatePosition(entityID);
    if (m_projection == "PERSPECTIVE")
        UpdateCamera(&m_camera, CAMERA_PERSPECTIVE);
    else
        UpdateCamera(&m_camera, CAMERA_ORBITAL);
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR)) {
        DrawGrid(10, 1.0f);
    }
}

void RLCamera3D::Start()
{
}

void RLCamera3D::OnLoad()
{
    auto engine = eng::Engine::GetEngine();
    DisableCursor();
    engine->pushPipeline([&]() {
        BeginMode3D(m_camera);
        m_mode3DEnabled = true;
    },
        -400);
    engine->pushPipeline([&]() {
        EndMode3D();
        m_mode3DEnabled = false;
    },
        400);
}

void RLCamera3D::OnAddComponent(int entityID)
{
    if (SYS.GetGraphicalModule()->GetID() != "GraphicalRayLib") {
        throw eng::CompatibilityException("RLCamera3D is only compatible with the RayLib graphical module");
    }
    m_camera = { { m_px, m_py, m_pz }, { m_tx, m_ty, m_tz }, { m_ux, m_uy, m_uz }, m_fov, 0 };
}

void RLCamera3D::SetTarget(int id)
{
    _targetID = id;
}

void RLCamera3D::SetTargetOffset(float x, float y, float z)
{
    _targetOffset = { x, y, z };
}

void RLCamera3D::updatePosition(int e)
{
    if (_targetID == -1)
        return;
    try {

    } catch (const std::exception& e) {
        CONSOLE::err << e.what() << std::endl;
    }
    auto& transform = SYS.GetComponent<CoreTransform>(e);
    auto& targetTransform = SYS.GetComponent<CoreTransform>(_targetID);
    float targetRotationY = -targetTransform.rotationY;
    m_camera.target = { targetTransform.x, targetTransform.y, targetTransform.z };

    graph::vec3f offset = _targetOffset;
    offset.x = _targetOffset.x * cosf(targetRotationY) - _targetOffset.z * sinf(targetRotationY);
    offset.z = _targetOffset.x * sinf(targetRotationY) + _targetOffset.z * cosf(targetRotationY);
    graph::vec3f position = {
        targetTransform.x + offset.x,
        targetTransform.y + offset.y,
        targetTransform.z + offset.z
    };
    m_camera.position = { position.x, position.y, position.z };
}