/*
** EPITECH PROJECT, 2023
** safe
** File description:
** RLCube.cpp
*/

#include "RLCube.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "metadata.hpp"
#include <raylib.h>
#include <raymath.h>

MANAGED_RESOURCE(RLCube)

RLCube::~RLCube()
{
    // TODO: Implement destructor
}

void RLCube::Update(int e)
{
    if (not _textureIsLoaded)
        loadTexture();
    try {
        auto transform = SYS.GetComponent<CoreTransform>(e);
        _mesh.transform = MatrixRotateXYZ({ transform.rotationX, transform.rotationY, transform.rotationZ });
        _mesh.transform = MatrixMultiply(_mesh.transform, MatrixScale(transform.scaleX, transform.scaleY, transform.scaleZ));
        _mesh.transform = MatrixMultiply(_mesh.transform, MatrixTranslate(transform.x, transform.y, transform.z));

        DrawModel(_mesh, { 0, 0, 0 }, 1.0f, WHITE);
    } catch (const std::exception& e) {
        std::cout << "RLCube::Update: " << e.what() << std::endl;
    }
}

void RLCube::OnAddComponent(int e)
{
    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    _mesh = LoadModelFromMesh(cubeMesh);
}

void RLCube::Start()
{
    // TODO: Implement Start
}

void RLCube::OnLoad()
{
    // TODO: Implement OnLoad
}

void RLCube::updateMeshFromTransform()
{
    // TODO: Implement updateMeshFromTransform
}

void RLCube::loadTexture()
{
    if (_texturePath == "")
        return;
    std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    std::string texturePath = ecs::ResourceManager::MakePath({ assetRoot, _texturePath });
    _texture = LoadTexture(texturePath.c_str());
    _mesh.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = _texture;
    _boundingBox = GetMeshBoundingBox(_mesh.meshes[0]);
    if (_texture.id != 0)
        _textureIsLoaded = true;
}