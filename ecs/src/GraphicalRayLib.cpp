/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** GraphicalRayLib.cpp
*/

#include "GraphicalRayLib/GraphicalRayLib.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include <iostream>
#include <raylib.h>

namespace raylib {
    std::string GraphicalRayLib::GetID() const
    {
        return "GraphicalRayLib";
    }

    void GraphicalRayLib::ModPipeline()
    {
        auto engine = eng::Engine::GetEngine();

        if (engine->IsOptionSet(eng::Engine::Options::NO_GRAPHICS))
            return;
        engine->pushPipeline([&]() {
            if (WindowShouldClose()) {
                CONSOLE::info << "Window closed" << std::endl;
                SYS.Exit();
            }
        },
            -999);
        engine->pushPipeline([&]() {
            BeginDrawing();
            ClearBackground(DARKGRAY);
        },
            -500);

        engine->pushPipeline([&]() {
            DisplayBuffer();
        },
            -10);

        engine->pushPipeline([&]() {
            EndDrawing();
        },
            500);
        CONSOLE::info << "GraphicalRayLib module loaded" << std::endl;
    }

    void GraphicalRayLib::Run(std::function<void(float deltaT)>&& func)
    {
        throw std::runtime_error("GraphicalRayLib::Run() is not implemented");
    }

    void GraphicalRayLib::Start()
    {
        if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::NO_GRAPHICS)) {
            CONSOLE::warn << "No graphics option set, skipping graphical module" << std::endl;
            return;
        }
        if (!eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::VERBOSE))
            SetTraceLogLevel(LOG_NONE);
        InitWindow(1920, 1080, "ECS");
        SetTargetFPS(60);

        InitAudioDevice();
        if (IsAudioDeviceReady() == false) {
            CONSOLE::err << "Error in initializing audio device" << std::endl;
        } else
            std::cout << "Audio device initialized" << std::endl;
    }

    void GraphicalRayLib::Stop()
    {
        if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::NO_GRAPHICS))
            return;
        if (IsAudioDeviceReady()) {
            CloseAudioDevice();
        }
        CloseWindow();
    }

    void GraphicalRayLib::RegisterEvent(const std::string& eventKey, std::function<void()>&& callback)
    {
        _events[eventKey] = callback;
    }

    graph::vec2f GraphicalRayLib::WindowGetMousePos() const
    {
        auto pos = SYS.GetInputManager().MousePosition();
        return { pos.x, pos.y };
    }

    bool GraphicalRayLib::WindowIsMouseRightPressed() const
    {
        return IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
    }

    bool GraphicalRayLib::WindowIsMouseLeftPressed() const
    {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    int GraphicalRayLib::WindowGetMouseWheelDelta() const
    {
        return GetMouseWheelMove();
    }

    int GraphicalRayLib::GetNextCharPressed()
    {
        return (int)SYS.GetInputManager().GetLastCharPressed();
    }

    bool GraphicalRayLib::isKeyPressed(int key)
    {
        return IsKeyPressed(key);
    }

    bool GraphicalRayLib::WindowIsMouseRightDown() const
    {
        return IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    }

    bool GraphicalRayLib::WindowIsMouseLeftDown() const
    {
        return IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    }

    bool GraphicalRayLib::CheckCollisionWithRectangle(graph::vec2f pos, Rectangle dimensions)
    {
        return CheckCollisionPointRec({ pos.x, pos.y }, dimensions);
    }

    void GraphicalRayLib::ClearBuffer()
    {
        _buffer.clear();
    }

    void GraphicalRayLib::DisplayBuffer()
    {
        for (auto& [priority, obj] : _buffer) {
            obj();
        }
        ClearBuffer();
    }

    void GraphicalRayLib::AddRectToBuffer(int priority, std::function<void()>&& func)
    {
        _buffer.insert({ priority, func });
    }
}
