/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** LightGraph.cpp
*/

#include "Engine.hpp"
#include "ECSImpl.hpp"
#include "LightGraph.hpp"

namespace graph
{
    std::string LightGraph::GetID() const
    {
        return "LightGraph";
    }

    void LightGraph::Run(std::function<void(float deltaT)> &&update)
    {
        throw std::runtime_error("Not implemented");
    }

    void LightGraph::Start()
    {
        _window = std::make_shared<lg::Window>();
    }

    void LightGraph::Stop()
    {
        if (_window == nullptr)
            return;
        _window->Close();
    }

    void LightGraph::ModPipeline()
    {
        auto engine = eng::Engine::GetEngine();
        engine->pushPipeline([&]() {
            if (_window->ShouldClose())
                std::cout << "Window closed" << std::endl;
                Sys.Exit();
        }, -999);
        engine->pushPipeline([&]() {
            _window->PollEvents();
            _window->Clear();
        }, -500);
        engine->pushPipeline([&]() {
            _window->Draw();
        }, 500);
    }

    void LightGraph::RegisterEvent(const std::string &eventKey, std::function<void()> &&callback)
    {
        _events[eventKey] = callback;
    }

    void LightGraph::WindowDrawRectangle(graphRect_t rectInfo)
    {
        throw std::runtime_error("Not implemented");
    }

    void LightGraph::WindowDrawCircle(graphCircle_t circleInfo)
    {
        throw std::runtime_error("Not implemented");
    }

    void LightGraph::WindowDrawText(graphText_t textInfo)
    {
        throw std::runtime_error("Not implemented");
    }

    vec2f LightGraph::WindowGetMousePos() const
    {
        throw std::runtime_error("Not implemented");
    }

    bool LightGraph::WindowIsMouseRightPressed() const
    {
        throw std::runtime_error("Not implemented");
    }

    bool LightGraph::WindowIsMouseLeftPressed() const
    {
        throw std::runtime_error("Not implemented");
    }

    bool LightGraph::WindowIsMouseRightDown() const
    {
        throw std::runtime_error("Not implemented");
    }

    bool LightGraph::WindowIsMouseLeftDown() const
    {
        throw std::runtime_error("Not implemented");
    }

    int LightGraph::WindowGetMouseWheelDelta() const
    {
        throw std::runtime_error("Not implemented");
    }

    int LightGraph::GetNextCharPressed()
    {
        throw std::runtime_error("Not implemented");
    }

    bool LightGraph::isKeyPressed(int key)
    {
        throw std::runtime_error("Not implemented");
    }
}
