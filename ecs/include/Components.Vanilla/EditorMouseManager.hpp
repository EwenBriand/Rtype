/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** EditorMouseManager.hpp
*/

#pragma once
#include <string>
#include "../Component.hpp"
#include "../IGraphicalModule.hpp"

class EditorMouseManager : public ecs::Component<EditorMouseManager> {
    public:
        void OnAddComponent(int entityID);
        void Update(int entityID);
        std::string GetClassName() const;
        void Save(const std::string &path);

    private:
        void setCtxtClosestEntity();
        void checkForDrag();

        graph::vec2f m_mousePrevPos = {0, 0};
        bool m_isPressed = false;
};
