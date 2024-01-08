/*
** EPITECH PROJECT, 2023
** safe
** File description:
** RLSphere.cpp
*/

#include "RLSphere.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "metadata.hpp"
#include <raylib.h>
#include <raymath.h>

MANAGED_RESOURCE(RLSphere)

RLSphere::~RLSphere()
{
}

void RLSphere::Update(int e)
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
        std::cout << "RLSphere::Update: " << e.what() << std::endl;
    }
}

void RLSphere::OnAddComponent(int e)
{
    Mesh cubeMesh = GenMeshSphere(1.0f, 32, 32);
    _mesh = LoadModelFromMesh(cubeMesh);
}

void RLSphere::Start()
{
}

void RLSphere::OnLoad()
{
}

void RLSphere::updateMeshFromTransform()
{
}

void RLSphere::loadTexture()
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