/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** RLMesh.cpp
*/

#include <raylib.h>
#include <raymath.h>
#include <thread>
#include "RLMesh.hpp"
#include "Engine.hpp"
#include "ECSImpl.hpp"
#include "ScopeLocation.hpp"
#include "Components.Vanilla/CoreTransform.hpp"

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

    DrawModel(m_model, {0, 0, 0}, 1.0f, WHITE);
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR)) {
        DrawBoundingBox(m_boundingBox, RED);
    }
}

void RLMesh::updatePosition(int entityID)
{
    try {
        auto transform = Sys.GetComponent<CoreTransform>(entityID);
        m_model.transform = MatrixRotateXYZ({transform.rotationX, transform.rotationY, transform.rotationZ});
        m_model.transform = MatrixMultiply(m_model.transform, MatrixScale(transform.scaleX, transform.scaleY, transform.scaleZ));
        m_model.transform = MatrixMultiply(m_model.transform, MatrixTranslate(transform.x, transform.y, transform.z));
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
    Console::info << "RLMesh module was loaded correctly" << std::endl;
}

void RLMesh::OnAddComponent(int e)
{
    if (Sys.GetGraphicalModule()->GetID() != "GraphicalRayLib") {
        throw eng::CompatibilityException("RLMesh is only compatible with the RayLib graphical module");
    }
    if (!m_modelIsLoaded)
        internalLoadObject();
}

void RLMesh::internalLoadObject()
{
    if (m_path == "" or m_modelIsLoaded) {
        std::cout << "not loading" << std::endl;
        return;
    }
    // loadModelAsync();
    doLoad();
}

bool RLMesh::doLoad() {
    std::string assetRoot = "";
        try {
            assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
            if (assetRoot[assetRoot.size() - 1] != '/')
                assetRoot += '/';
        } catch (std::exception) {
            Console::warn << "No asset directory specified in config, using current directory" << std::endl;
            assetRoot = "./";
        }
        std::string pathStr = assetRoot + m_path;
        while (pathStr[pathStr.size() - 1] == '\n' or pathStr[pathStr.size() - 1] == ' ')
            pathStr = pathStr.substr(0, pathStr.size() - 1);
        std::filesystem::path path = pathStr;
        if (not std::filesystem::exists(path)) {
            Console::err << "Asset [" << path << "] not found" << std::endl;
            return false;
        }
        withLocation(path) {
            m_model = LoadModel(("./" + path.filename().string()).c_str());
        }
        m_boundingBox = GetMeshBoundingBox(m_model.meshes[0]);
        if (errno != 0) {
            Console::err << "Failed to load model " << path << std::endl;
            perror("Error: ");
            m_path = "";
            return false;
        }
        m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m_texture;
        m_modelIsLoaded = true;
        return true;
}

void RLMesh::loadModelAsync() {
    m_modelLoader = std::async(std::launch::async, [this]() {
        std::cout << "launched"<< std::endl;
        doLoad();
        while (m_modelLoader.valid() && m_modelLoader.wait_for(std::chrono::milliseconds(1)) != std::future_status::ready);
        // while (not m_modelIsLoaded);
        std::cout << "ok! loaded the mesh!" << std::endl;
    });
}

// [[nodiscard]]
// bool RLMesh::internalLoadTexture(const std::string &assetRoot)
// {
//     if (m_texturePath == "")
//         return false;

//     std::string path = assetRoot + m_texturePath;
//     while (path[path.size() - 1] == '\n' or path[path.size() - 1] == ' ')
//         path = path.substr(0, path.size() - 1);
//     if (not std::filesystem::exists(path)) {
//         Console::err << "Asset [" << path << "] not found" << std::endl;
//         return false;
//     }
//     Console::info << "Loading texture " << path << std::endl;
//     m_texture = LoadTexture(path.c_str());
//     return true;
// }
