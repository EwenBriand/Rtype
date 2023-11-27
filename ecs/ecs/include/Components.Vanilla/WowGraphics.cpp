/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** WowGraphics.cpp
*/

#include <string>
#include <raylib.h>
#include "Components.Vanilla/WowGraphics.hpp"
#include "Engine.hpp"
#include "Components.User/RLCamera3D.hpp"
#include "ECSImpl.hpp"
#include "ShaderBuildSystem.hpp"
#include "ShaderBuilder.hpp"

namespace wow {
    std::shared_ptr<WowGraphicsEngine> WowGraphicsEngine::Instance = nullptr;

    std::shared_ptr<WowGraphicsEngine> WowGraphicsEngine::Get()
    {
        return Instance;
    }

    void WowGraphicsEngine::Enable()
    {
        Instance = std::make_shared<WowGraphicsEngine>();
    }

    WowGraphicsEngine::WowGraphicsEngine() :
        _camera(Sys.AddComponent(Sys.GetSystemHolder(), "RLCamera3D")),
        _vao(std::make_shared<lg::VAO>()),
        _screenQuadVBO(std::make_shared<lg::VBO>(_screenQuadVertices, sizeof(_screenQuadVertices))),
        _screenQuadEBO(std::make_shared<lg::EBO>(_screenQuadIndices, sizeof(_screenQuadIndices)))
    {
        if (Instance != nullptr)
            throw std::runtime_error("WowGraphicsEngine is already enabled");
        auto graphicsModule = eng::Engine::GetEngine()->GetGraphicalModule();
        if (graphicsModule->GetID() != "GraphicalRayLib")
            throw std::runtime_error("WowGraphicsEngine can only be enabled with GraphicalRayLib");

        auto engine = eng::Engine::GetEngine();
        engine->pushPipeline([&]() {
            StartRenderMode();
        }, -200);
        engine->pushPipeline([&]() {
            EndRenderMode();
        }, -150);

        InitWowGraphicsEngine();
    }

    WowGraphicsEngine::~WowGraphicsEngine()
    {
        UnloadShader(_raymarchingShader);
    }

    void WowGraphicsEngine::StartRenderMode()
    {
        // BeginShaderMode(_raymarchingShader);
        // _vao->Bind();
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // _vao->Unbind();
        auto prog = *(lg::ShaderBuildSystem::Get()["spriteShader"].Get());
        prog.Use();
        _sprite->Draw();
    }

    void WowGraphicsEngine::EndRenderMode()
    {
        // EndShaderMode();
    }

    void WowGraphicsEngine::InitWowGraphicsEngine()
    {
        _raymarchingShader = LoadShader(0, Sys.GetResourceManager().LoadFileText(
            "shaders/raymarching/fragment.glsl"
        ).c_str());

        _sprite = std::make_shared<lg::Sprite>("../../assets/textures/wall.jpg");
        lg::ShaderBuildSystem::Get().BuildAll();
    }
}
