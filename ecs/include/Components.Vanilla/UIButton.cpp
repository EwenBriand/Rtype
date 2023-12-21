/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIButton.cpp
*/

#include "UIButton.hpp"
#include "../ECSImpl.hpp"
#include "CLI.hpp"

decltype(UIButton::AvailableActionsClick) UIButton::AvailableActionsClick;

void UIButton::Update(int entityID)
{
    if (m_clickCallback == nullptr || (m_clickCallbackName != m_prevClickCallbackName)) {
        m_clickCallbackName.erase(m_clickCallbackName.find_last_not_of(" \n\r\t") + 1);
        if (AvailableActionsClick.find(m_clickCallbackName) != AvailableActionsClick.end()) {
            m_clickCallback = AvailableActionsClick[m_clickCallbackName];
        }
        m_prevClickCallbackName = m_clickCallbackName;
    }
    TestClick();
    graph::graphText_t textInfo = { .pos = { m_uiDiv->GetPosition().x + 5, m_uiDiv->GetPosition().y + 2 },
        .text = m_text,
        .color = { 255, 255, 255, 255 },
        .fontSize = 12 };
    SYS.GetGraphicalModule()->AddRectToBuffer(1, [textInfo]() {
        DrawText(textInfo.text.c_str(), textInfo.pos.x, textInfo.pos.y, textInfo.fontSize, { textInfo.color.x, textInfo.color.y, textInfo.color.z, textInfo.color.w });
    });
}

void UIButton::OnAddComponent(int entityID)
{
    UIDiv* uiDiv = nullptr;

    try {
        uiDiv = &(SYS.GetComponent<UIDiv>(entityID));
    } catch (std::exception& e) {
        SYS.AddComponent<UIDiv>(entityID);
        uiDiv = &(SYS.GetComponent<UIDiv>(entityID));
    }
    m_uiDiv = uiDiv;
    m_hoverDiv = uiDiv;
    m_clickable = true;
    SetHoverCallback([&] {
        graph::vec2f mousePos = SYS.GetGraphicalModule()->WindowGetMousePos();
        graph::graphText_t textInfo = { .pos = { mousePos.x + 10, mousePos.y + 10 },
            .text = m_description,
            .color = { 255, 255, 255, 255 },
            .fontSize = 12 };
        SYS.GetGraphicalModule()->AddRectToBuffer(1, [textInfo]() {
            DrawText(textInfo.text.c_str(), textInfo.pos.x, textInfo.pos.y, textInfo.fontSize, { textInfo.color.x, textInfo.color.y, textInfo.color.z, textInfo.color.w });
        });
    });
}

void UIButton::RegisterCallback(const std::string& name, std::function<void()>&& callback)
{
    AvailableActionsClick[name] = callback;
}

void UIButton::OnLoad()
{
    Entity systemHolder = SYS.GetSystemHolder();
    try {
        auto& cli = SYS.GetComponent<CLI>(systemHolder);
        cli.RegisterCustomCommand("bt-listcb", [&](CLI& c, std::vector<std::string> args) {
            CONSOLE::info << "Available callbacks:" << std::endl;
            for (auto& cb : AvailableActionsClick)
                CONSOLE::info << "\t - " << cb.first << std::endl;
        });
        cli.RegisterCustomCommand("bt-setcb", [&](CLI& c, std::vector<std::string> args) {
            Entity ctxt = c.GetContext();
            try {
                auto& bt = SYS.GetComponent<UIButton>(ctxt);
                bt.m_clickCallbackName = args[0];
            } catch (std::exception& e) {
                CONSOLE::warn << "No UIButton component on entity " << ctxt << std::endl;
            }
        });
    } catch (std::exception& e) {
        CONSOLE::warn << "No Cli, cannot change commands for UIButton" << std::endl;
    }
}
