/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIMenu.cpp
*/

#include "../ECSImpl.hpp"
#include "UIMenu.hpp"

void UIMenu::Update(int entityID)
{
    if (!m_isVisible && !m_alwaysVisible)
        return;
    m_hoverDiv->Update(entityID);
}

void UIMenu::OnAddComponent(int entityID)
{
    UIDiv *uiDiv = nullptr;

    Sys.AddComponent<UIDiv>(entityID);
    uiDiv = &(Sys.GetComponent<UIDiv>(entityID));
    m_hoverDiv = uiDiv;
}

void UIMenu::SetAlwaysVisible(bool alwaysVisible)
{
    m_alwaysVisible = alwaysVisible;
}

