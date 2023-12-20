/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** GraphicalRayLib.hpp
*/

#ifndef E738593F_2DE8_44E5_8093_0E4760B3FD35
#define E738593F_2DE8_44E5_8093_0E4760B3FD35
#include "IGraphicalModule.hpp"
#include <functional>
#include <map>
#include <string>

namespace raylib {
    class GraphicalRayLib : public graph::IGraphicalModule {
    public:
        std::string GetID() const override;
        void Run(std::function<void(float deltaT)>&& func) override;
        void Start() override;
        void Stop() override;
        void ModPipeline() override;
        void RegisterEvent(const std::string& eventKey, std::function<void()>&& callback) override;
        void WindowDrawRectangle(graph::graphRect_t rectInfo, int prio) override;
        void WindowDrawCircle(graph::graphCircle_t circleInfo, int prio) override;
        void WindowDrawText(graph::graphText_t textInfo, int prio) override;

        graph::vec2f WindowGetMousePos() const override;
        bool WindowIsMouseRightPressed() const override;
        bool WindowIsMouseLeftPressed() const override;
        bool WindowIsMouseRightDown() const override;
        bool WindowIsMouseLeftDown() const override;
        int WindowGetMouseWheelDelta() const override;

        int GetNextCharPressed() override;
        bool isKeyPressed(int key) override;
        bool CheckCollisionWithRectangle(graph::vec2f pos, Rectangle dimensions) override;

        template <typename T>
        void AddRectToBuffer(T obj, int priority);

        void ClearBuffer() override;
        void DisplayBuffer() override;

    private:
        std::map<std::string, std::function<void()>> _events;
        std::map<int, std::function<void()>, std::greater<int>> _buffer;
    };
}

#endif /* E738593F_2DE8_44E5_8093_0E4760B3FD35 */
