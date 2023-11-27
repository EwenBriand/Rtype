/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** LightGraph.hpp
*/

#pragma once

#include <map>
#include <string>
#include "Types.hpp"
#include "../IGraphicalModule.hpp"
#include "Window.hpp"

namespace graph {
    class LightGraph : IGraphicalModule {
        public:
            std::string GetID() const override;
            void Run(std::function<void(float deltaT)> &&callback) override;
            void Start() override;
            void Stop() override;

            void ModPipeline() override;

            void RegisterEvent(const std::string &eventKey, std::function<void()> &&callback) override;
            void WindowDrawRectangle(graphRect_t rectInfo) override;
            void WindowDrawCircle(graphCircle_t circleInfo) override;
            void WindowDrawText(graphText_t textInfo) override;

            vec2f WindowGetMousePos() const override;
            bool WindowIsMouseRightPressed() const override;
            bool WindowIsMouseLeftPressed() const override;
            bool WindowIsMouseRightDown() const override;
            bool WindowIsMouseLeftDown() const override;
            int WindowGetMouseWheelDelta() const override;

            int GetNextCharPressed() override;
            bool isKeyPressed(int key) override;
        private:
            std::shared_ptr<lg::Window> _window;
            std::map<std::string, Action> _events;
    };
}
