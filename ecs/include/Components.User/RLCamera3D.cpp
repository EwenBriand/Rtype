/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** RLCamera3D.cpp.
*/

#include "RLCamera3D.hpp"
#include "Engine.hpp"
#include "ECSImpl.hpp"

MANAGED_RESOURCE(RLCamera3D)

RLCamera3D::~RLCamera3D() {
    std::cout << "destroy camera" << std::endl;
    if (m_mode3DEnabled)
        EndMode3D();
    std::cout << "destroyed camera" << std::endl;
}

void RLCamera3D::Update(int entityID) {
    UpdateCamera(&m_camera, CAMERA_PERSPECTIVE);
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR)) {
        DrawGrid(10, 1.0f);
    }
}

void RLCamera3D::Start() {

}

void RLCamera3D::OnLoad() {
    auto engine = eng::Engine::GetEngine();
    DisableCursor();
    engine->pushPipeline([&]() {
        BeginMode3D(m_camera);
        m_mode3DEnabled = true;
    }, -400);
    engine->pushPipeline([&]() {
        EndMode3D();
        m_mode3DEnabled = false;
    }, 400);
}

void RLCamera3D::OnAddComponent(int entityID) {
    if (SYS.GetGraphicalModule()->GetID() != "GraphicalRayLib") {
        throw eng::CompatibilityException("RLCamera3D is only compatible with the RayLib graphical module");
    }
    m_camera = { { m_px, m_py, m_pz }, { m_tx, m_ty, m_tz }, { m_ux, m_uy, m_uz }, m_fov, 0 };
}
