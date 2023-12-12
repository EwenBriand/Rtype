/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIScrollableList.hpp
*/

#pragma once
#include <string>
#include <map>
#include "../Component.hpp"
#include "metadata.hpp"
#include "../IGraphicalModule.hpp"
#include "UIMenu.hpp"
#include "../Types.hpp"
#include "../IGraphicalModule.hpp"

class UIScrollableList {
    public:
        void OnAddComponent(int entityID);

        void Draw();
        void SetHeight(int height);
        void AddOptions(std::map<std::string, Action> options);
        void SetPosition(graph::vec2f pos);
        std::map<std::string, Action> GetOptions() const;
    private:

        graph::vec2f m_pos;
        int m_height = 0;
        int m_scroll = 0;
        std::map<std::string, Action> m_options;
};
