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

    private:
        std::map<std::string, std::function<void()>> _events;
        std::multimap<int, std::function<void()>, std::less<int>> _buffer;
    };
}

#endif /* E738593F_2DE8_44E5_8093_0E4760B3FD35 */
