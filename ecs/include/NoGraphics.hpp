/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** NoGraphics.hpp
*/

#pragma once

#include "IGraphicalModule.hpp"

namespace eng {
    /**
     * @brief A dummy class to replace the graphical module when the
     * ecs is started with the --no-graphics option
     *
     */
    class NoGraphics : public graph::IGraphicalModule {
    public:
        std::string GetID() const override;
        void Run(std::function<void(float deltaT)>&& func) override;
        void Start() override;
        void Stop() override;
        void ModPipeline() override;
        void RegisterEvent(const std::string& eventKey, std::function<void()>&& callback) override;

        graph::vec2f WindowGetMousePos() const override;
        bool WindowIsMouseRightPressed() const override;
        bool WindowIsMouseLeftPressed() const override;
        bool WindowIsMouseRightDown() const override;
        bool WindowIsMouseLeftDown() const override;
        int WindowGetMouseWheelDelta() const override;

        int GetNextCharPressed() override;
        bool isKeyPressed(int key) override;
        bool CheckCollisionWithRectangle(graph::vec2f pos, Rectangle dimensions) override;

        void AddRectToBuffer(int priority, std::function<void()>&& func) override;

        void ClearBuffer() override;
        void DisplayBuffer() override;

        graph::vec2i GetScreenSize() const override;

    private:
    };
} // namespace eng
