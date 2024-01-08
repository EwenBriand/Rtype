/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** RLCamera3D.cpp.
*/

#include "RLCamera3D.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(RLCamera3D)

RLCamera3D::~RLCamera3D()
{
    if (m_mode3DEnabled)
        EndMode3D();
}

void RLCamera3D::Update(int entityID)
{
    updatePosition();
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
    std::cout << "RLCamera3D::OnAddComponent" << std::endl;
    if (SYS.GetGraphicalModule()->GetID() != "GraphicalRayLib") {
        throw eng::CompatibilityException("RLCamera3D is only compatible with the RayLib graphical module");
    }
    std::cout << "RLCamera3D::OnAddComponent 2" << std::endl;
    m_camera = { { m_px, m_py, m_pz }, { m_tx, m_ty, m_tz }, { m_ux, m_uy, m_uz }, m_fov, 0 };
    std::cout << "done" << std::endl;
}

void RLCamera3D::SetTarget(int entity)
{
    m_target = entity;
}

void RLCamera3D::SetTargetOffset(float x, float y, float z)
{
    m_targetOffset = { x, y, z };
}

void RLCamera3D::updatePosition()
{
    if (m_target == -1)
        return;
    try {
        auto transform = SYS.GetComponent<CoreTransform>(m_target);
        m_camera.target = {
            transform.x,
            transform.y,
            transform.z
        };
        auto forward = transform.GetForward();
        auto offset = m_targetOffset;
        float rotation = -transform.rotationY;

        offset = {
            offset.x * cosf(rotation) - offset.z * sinf(rotation),
            offset.y,
            offset.x * sinf(rotation) + offset.z * cosf(rotation)
        };
        m_camera.position = {
            transform.x + offset.x,
            transform.y + offset.y,
            transform.z + offset.z
        };

    } catch (std::exception) {
        return;
    }
}