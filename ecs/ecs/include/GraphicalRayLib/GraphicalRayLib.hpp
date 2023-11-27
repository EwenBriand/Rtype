/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** GraphicalRayLib.hpp
*/

#ifndef E738593F_2DE8_44E5_8093_0E4760B3FD35
#define E738593F_2DE8_44E5_8093_0E4760B3FD35
#include <functional>
#include <string>
#include <map>
#include "IGraphicalModule.hpp"

namespace raylib {
    class GraphicalRayLib : public graph::IGraphicalModule {
        public:
            std::string GetID() const override;
            void Run(std::function<void(float deltaT)> &&func) override;
            void Start() override;
            void Stop() override;
            void ModPipeline() override;
            void RegisterEvent(const std::string &eventKey, std::function<void()> &&callback) override;
            void WindowDrawRectangle(graph::graphRect_t rectInfo) override;
            void WindowDrawCircle(graph::graphCircle_t circleInfo) override;
            void WindowDrawText(graph::graphText_t textInfo) override;

            graph::vec2f WindowGetMousePos() const override;
            bool WindowIsMouseRightPressed() const override;
            bool WindowIsMouseLeftPressed() const override;
            bool WindowIsMouseRightDown() const override;
            bool WindowIsMouseLeftDown() const override;
            int WindowGetMouseWheelDelta() const override;

            int GetNextCharPressed() override;
            bool isKeyPressed(int key) override;
        private:
            std::map<std::string, std::function<void()>> _events;
    };
}

#endif /* E738593F_2DE8_44E5_8093_0E4760B3FD35 */
