/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIScrollableList.cpp
*/

#include "UIScrollableList.hpp"
#include "ECSImpl.hpp"

void UIScrollableList::OnAddComponent(int entityID)
{
}

void UIScrollableList::SetHeight(int height)
{
    m_height = height;
}

void UIScrollableList::SetPosition(graph::vec2f pos)
{
    m_pos = pos;
}

void UIScrollableList::Draw()
{
    throw std::runtime_error("UIScrollableList::Draw: not implemented");
    // int i = 0;
    // auto graphics = SYS.GetGraphicalModule();
    // int start = 0;
    // int end = m_options.size() - 1;
    // int mouseWheelDelta = graphics->WindowGetMouseWheelDelta();

    // if (mouseWheelDelta != 0) {
    //     m_scroll += mouseWheelDelta;
    // }
    // if (m_scroll < 0)
    //     m_scroll = 0;
    // if (m_scroll > end - m_height)
    //     m_scroll = end - m_height;
    // start = m_scroll;
    // end = m_scroll + m_height;
    // if (end > m_options.size())
    //     end = m_options.size() - 1;
    // for (auto &option : m_options) {
    //     if (i >= start && i <= end) {
    //         if (m_isFocused) {
    //             graphics->WindowDrawRectangle({
    //                 .pos = {m_pos.x, m_pos.y + (i - start) * 20},
    //                 .dimensions = {200, 20},
    //                 .bgColor = {33, 33, 33, 255}
    //             });
    //         }
    //         graphics->WindowDrawText({
    //             .pos = {m_pos.x + 5, m_pos.y + (i - start) * 20 + 2},
    //             .text = option.first,
    //             .color = {255, 255, 255, 255},
    //             .fontSize = 12
    //         },5);
    //     }
    //     ++i;
    // }
}

void UIScrollableList::AddOptions(std::map<std::string, Action> options)
{
    for (auto& option : options) {
        if (m_options.find(option.first) != m_options.end()) {
            CONSOLE::warn << "UIScrollableList::AddOptions: option " << option.first << " already exists" << std::endl;
            continue;
        }
        m_options[option.first] = option.second;
    }
}

std::map<std::string, Action> UIScrollableList::GetOptions() const
{
    return m_options;
}
