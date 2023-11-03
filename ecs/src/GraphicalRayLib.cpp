/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** GraphicalRayLib.cpp
*/

#include <iostream>
#include <raylib.h>
#include "GraphicalRayLib/GraphicalRayLib.hpp"
#include "Engine.hpp"
#include "ECSImpl.hpp"

namespace raylib {
    std::string GraphicalRayLib::GetID() const
    {
        return "GraphicalRayLib";
    }

    void GraphicalRayLib::ModPipeline()
    {
        auto engine = eng::Engine::GetEngine();
        engine->pushPipeline([&]() {
            if (WindowShouldClose()) {
                Console::info << "Window closed" << std::endl;
                Sys.Exit();
            }
        }, -999);
        engine->pushPipeline([&]() {
            BeginDrawing();
            ClearBackground(RAYWHITE);
        }, -500);

        engine->pushPipeline([&]() {
            EndDrawing();
        }, 500);
    }

    void GraphicalRayLib::Run(std::function<void(float deltaT)> &&func)
    {
        throw std::runtime_error("GraphicalRayLib::Run() is not implemented");
    }

    void GraphicalRayLib::Start()
    {
        SetTraceLogLevel(LOG_ERROR);
        InitWindow(1920, 1080, "ECS");
        SetTargetFPS(60);
    }

    void GraphicalRayLib::Stop()
    {
        CloseWindow();
    }

    void GraphicalRayLib::RegisterEvent(const std::string &eventKey, std::function<void()> &&callback)
    {
        _events[eventKey] = callback;
    }

    void GraphicalRayLib::WindowDrawRectangle(graph::graphRect_t rectInfo)
    {
        DrawRectangle(rectInfo.pos.x, rectInfo.pos.y, rectInfo.dimensions.x, rectInfo.dimensions.y, {rectInfo.bgColor.x, rectInfo.bgColor.y, rectInfo.bgColor.z, rectInfo.bgColor.w});
        DrawRectangleLinesEx({rectInfo.pos.x, rectInfo.pos.y, rectInfo.dimensions.x, rectInfo.dimensions.y}, rectInfo.borderSize, {rectInfo.borderColor.x, rectInfo.borderColor.y, rectInfo.borderColor.z, 255});
    }

    void GraphicalRayLib::WindowDrawCircle(graph::graphCircle_t circleInfo)
    {
        DrawCircle(circleInfo.pos.x, circleInfo.pos.y, circleInfo.radius, {circleInfo.color.x, circleInfo.color.y, circleInfo.color.z, circleInfo.color.w});
        DrawCircleLines(circleInfo.pos.x, circleInfo.pos.y, circleInfo.radius, {circleInfo.borderColor.x, circleInfo.borderColor.y, circleInfo.borderColor.z, circleInfo.borderColor.w});
    }

    void GraphicalRayLib::WindowDrawText(graph::graphText_t textInfo)
    {
        DrawText(textInfo.text.c_str(), textInfo.pos.x, textInfo.pos.y, textInfo.fontSize, {textInfo.color.x, textInfo.color.y, textInfo.color.z, textInfo.color.w});
    }

    graph::vec2f GraphicalRayLib::WindowGetMousePos() const
    {
        auto pos = GetMousePosition();
        return {pos.x, pos.y};
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
        int c = GetCharPressed();
        if (c == 0)
            c = -1;
        return c;
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
}
