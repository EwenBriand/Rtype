/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** RLMesh.cpp
*/

#include "RLMesh.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "ScopeLocation.hpp"
#include <raylib.h>
#include <raymath.h>
#include <thread>

MANAGED_RESOURCE(RLMesh)

RLMesh::~RLMesh()
{
    if (m_modelIsLoaded)
        UnloadModel(m_model);
    if (m_textureIsLoaded)
        UnloadTexture(m_texture);
}

void RLMesh::Update(int entityID)
{
    if (!m_modelIsLoaded) {
        if (!m_loadingStarted && m_path != "") {
            internalLoadObject();
            m_loadingStarted = true;
            return;
        }
        return;
    }

    updatePosition(entityID);

    DrawModel(m_model, { 0, 0, 0 }, 1.0f, WHITE);
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR)) {
        DrawBoundingBox(m_boundingBox, RED);
    }
}

void RLMesh::updatePosition(int entityID)
{
    try {
        auto transform = SYS.GetComponent<CoreTransform>(entityID);
        m_model.transform = MatrixRotateXYZ({ transform.rotationX, transform.rotationY, transform.rotationZ });
        m_model.transform = MatrixMultiply(m_model.transform, MatrixScale(transform.scaleX, transform.scaleY, transform.scaleZ));
        m_model.transform = MatrixMultiply(m_model.transform, MatrixTranslate(transform.x, transform.y, transform.z));
        m_boundingBox = GetMeshBoundingBox(m_model.meshes[0]);

    } catch (std::exception) {
        return;
    }
}

void RLMesh::Start()
{
    if (m_path == "")
        return;
    internalLoadObject();
}

void RLMesh::OnLoad()
{
    CONSOLE::info << "RLMesh module was loaded correctly" << std::endl;
}

void RLMesh::OnAddComponent(int e)
{
    if (SYS.GetGraphicalModule()->GetID() != "GraphicalRayLib") {
        throw eng::CompatibilityException("RLMesh is only compatible with the RayLib graphical module");
    }
    if (!m_modelIsLoaded)
        internalLoadObject();
}

void RLMesh::internalLoadObject()
{
    if (m_path == "" or m_modelIsLoaded) {
        return;
    }

    doLoad();
}

bool RLMesh::doLoad()
{
    std::string assetRoot = "";
    try {
        assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
        if (assetRoot[assetRoot.size() - 1] != '/')
            assetRoot += '/';
    } catch (std::exception) {
        CONSOLE::warn << "No asset directory specified in config, using current directory" << std::endl;
        assetRoot = "./";
    }
    std::string pathStr = assetRoot + m_path;
    while (pathStr[pathStr.size() - 1] == '\n' or pathStr[pathStr.size() - 1] == ' ')
        pathStr = pathStr.substr(0, pathStr.size() - 1);
    std::filesystem::path path = pathStr;
    if (not std::filesystem::exists(path)) {
        CONSOLE::err << "Asset [" << path << "] not found" << std::endl;
        return false;
    }
    std::cout << "loading model from path: " << path << std::endl;
    withLocation(path)
    {
        m_model = LoadModel((std::string("./") + path.filename().string()).c_str());
    }
    std::cout << "quiche" << std::endl;
    m_boundingBox = GetMeshBoundingBox(m_model.meshes[0]);
    std::cout << "poireau" << std::endl;

    m_modelIsLoaded = true;
    return true;
}

void RLMesh::loadModelAsync()
{
    m_modelLoader = std::async(std::launch::async, [this]() {
        doLoad();
        while (m_modelLoader.valid() && m_modelLoader.wait_for(std::chrono::milliseconds(1)) != std::future_status::ready)
            ;
        // while (not m_modelIsLoaded);
        std::cout << "ok! loaded the mesh!" << std::endl;
    });
}
