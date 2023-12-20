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
            1);

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

    void GraphicalRayLib::WindowDrawRectangle(graph::graphRect_t rectInfo, int prio)
    {
        _buffer[prio] = [rectInfo]() {
            DrawRectangle(rectInfo.pos.x, rectInfo.pos.y, rectInfo.dimensions.x, rectInfo.dimensions.y, { rectInfo.bgColor.x, rectInfo.bgColor.y, rectInfo.bgColor.z, rectInfo.bgColor.w });
            DrawRectangleLinesEx({ rectInfo.pos.x, rectInfo.pos.y, rectInfo.dimensions.x, rectInfo.dimensions.y }, rectInfo.borderSize, { rectInfo.borderColor.x, rectInfo.borderColor.y, rectInfo.borderColor.z, 255 });
        };
    }

    void GraphicalRayLib::WindowDrawCircle(graph::graphCircle_t circleInfo, int prio)
    {
        _buffer[prio] = [circleInfo]() {
            DrawCircle(circleInfo.pos.x, circleInfo.pos.y, circleInfo.radius, { circleInfo.color.x, circleInfo.color.y, circleInfo.color.z, circleInfo.color.w });
            DrawCircleLines(circleInfo.pos.x, circleInfo.pos.y, circleInfo.radius, { circleInfo.borderColor.x, circleInfo.borderColor.y, circleInfo.borderColor.z, circleInfo.borderColor.w });
        };
    }

    void GraphicalRayLib::WindowDrawText(graph::graphText_t textInfo, int prio)
    {
        _buffer[prio] = [textInfo]() { DrawText(textInfo.text.c_str(), textInfo.pos.x, textInfo.pos.y, textInfo.fontSize, { textInfo.color.x, textInfo.color.y, textInfo.color.z, textInfo.color.w }); };
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
    }

    template <typename T>
    void GraphicalRayLib::AddRectToBuffer(T obj, int priority)
    {
        if (std::is_same<decltype(obj), graph::graphText_t>::value)
            _buffer[priority] = [obj]() {
                DrawText(obj.text.c_str(), obj.pos.x, obj.pos.y, obj.fontSize, { obj.color.x, obj.color.y, obj.color.z, obj.color.w });
            };
        else if (std::is_same<decltype(obj), graph::graphCircle_t>::value)
            _buffer[priority] = [obj]() {
                DrawCircle(obj.pos.x, obj.pos.y, obj.radius, { obj.color.x, obj.color.y, obj.color.z, obj.color.w });
                DrawCircleLines(obj.pos.x, obj.pos.y, obj.radius, { obj.borderColor.x, obj.borderColor.y, obj.borderColor.z, obj.borderColor.w });
            };
        else if (std::is_same<decltype(obj), graph::graphRect_t>::value)
            _buffer[priority] = [obj]() {
                DrawRectangle(obj.pos.x, obj.pos.y, obj.dimensions.x, obj.dimensions.y, { obj.bgColor.x, obj.bgColor.y, obj.bgColor.z, obj.bgColor.w });
                DrawRectangleLinesEx({ obj.pos.x, obj.pos.y, obj.dimensions.x, obj.dimensions.y }, obj.borderSize, { obj.borderColor.x, obj.borderColor.y, obj.borderColor.z, 255 });
            };
    }
}
