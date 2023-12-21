/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** NoGraphics.cpp
*/

#include "NoGraphics.hpp"
#include <stdexcept>

namespace eng {
    std::string NoGraphics::GetID() const
    {
        return "NoGraphics";
    }

    void NoGraphics::Run(std::function<void(float deltaT)>&& func)
    {
        throw std::runtime_error("NoGraphics::Run() is not implemented");
    }

    void NoGraphics::Start()
    {
    }

    void NoGraphics::Stop()
    {
    }

    void NoGraphics::ModPipeline()
    {
    }

    void NoGraphics::RegisterEvent(const std::string& eventKey, std::function<void()>&& callback)
    {
    }

    void NoGraphics::WindowDrawRectangle(graph::graphRect_t rectInfo, int prio)
    {
    }

    void NoGraphics::WindowDrawCircle(graph::graphCircle_t circleInfo, int prio)
    {
    }

    void NoGraphics::WindowDrawText(graph::graphText_t textInfo, int prio)
    {
    }

    void NoGraphics::WindowDrawTexture(graph::graphTexture_t spriteInfo, int prio)
    {
    }

    graph::vec2f NoGraphics::WindowGetMousePos() const
    {
        return { 0, 0 };
    }

    bool NoGraphics::WindowIsMouseRightPressed() const
    {
        return false;
    }

    bool NoGraphics::WindowIsMouseLeftPressed() const
    {
        return false;
    }

    bool NoGraphics::WindowIsMouseRightDown() const
    {
        return false;
    }

    bool NoGraphics::WindowIsMouseLeftDown() const
    {
        return false;
    }

    int NoGraphics::WindowGetMouseWheelDelta() const
    {
        return 0;
    }

    int NoGraphics::GetNextCharPressed()
    {
        return 0;
    }

    bool NoGraphics::isKeyPressed(int key)
    {
        return false;
    }

    bool NoGraphics::CheckCollisionWithRectangle(graph::vec2f pos, Rectangle dimensions)
    {
        return false;
    }

    void NoGraphics::ClearBuffer()
    {
    }

    void NoGraphics::DisplayBuffer()
    {
    }

} // namespace eng
