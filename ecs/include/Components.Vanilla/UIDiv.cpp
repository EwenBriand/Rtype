/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIDiv.cpp
*/

#include "UIDiv.hpp"
#include "../ECSImpl.hpp"

void UIDiv::Update(__attribute__((unused)) int entityID)
{
    if (m_visible)
        Draw();
}

void UIDiv::SetColor(graph::vec4uc color)
{
    m_r = color.x;
    m_g = color.y;
    m_b = color.z;
    m_a = color.w;
}

void UIDiv::SetPosition(graph::vec2f position)
{
    m_x = position.x;
    m_y = position.y;
}

void UIDiv::SetSize(graph::vec2f size)
{
    m_width = size.x;
    m_height = size.y;
}

void UIDiv::SetBorder(graph::vec4uc color, int thickness)
{
    m_border_r = color.x;
    m_border_g = color.y;
    m_border_b = color.z;
    m_border_a = color.w;
    m_border_thickness = thickness;
}

void UIDiv::OnAddComponent(int entityID)
{
    m_parentID = entityID;
}

void UIDiv::SetParentID(int parentID)
{
    m_parentID = parentID;
}

graph::vec2f UIDiv::GetPosition() const
{
    int dx, dy = 0;

    try {
        auto transform = SYS.GetComponent<CoreTransform>(m_parentID);
        dx = transform.x;
        dy = transform.y;
    } catch (std::exception& e) {
        dx = 0;
        dy = 0;
    }
    return { dx + m_x, dy + m_y };
}

graph::vec2f UIDiv::GetSize() const
{
    return { m_width, m_height };
}

void UIDiv::Draw() const
{
    int dx, dy = 0;
    try {
        auto transform = SYS.GetComponent<CoreTransform>(m_parentID);
        dx = transform.x;
        dy = transform.y;
    } catch (std::exception& e) {
        dx = 0;
        dy = 0;
    }
    SYS.GetGraphicalModule()->WindowDrawRectangle({ .pos = { dx + m_x, dy + m_y },
                                                      .dimensions = { m_width, m_height },
                                                      .bgColor = {
                                                          static_cast<unsigned char>(m_r),
                                                          static_cast<unsigned char>(m_g),
                                                          static_cast<unsigned char>(m_b),
                                                          static_cast<unsigned char>(m_a) },
                                                      .borderColor = { static_cast<unsigned char>(m_border_r), static_cast<unsigned char>(m_border_g), static_cast<unsigned char>(m_border_b), static_cast<unsigned char>(m_border_a) },
                                                      .borderSize = m_border_thickness },
        2);
}

void UIDiv::setVisible(bool visible)
{
    m_visible = visible;
}
