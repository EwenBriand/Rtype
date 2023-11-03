/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIButton.cpp
*/

#include "../ECSImpl.hpp"
#include "UIButton.hpp"
#include "CLI.hpp"

decltype(UIButton::AvailableActionsClick) UIButton::AvailableActionsClick;

void UIButton::Update(int entityID)
{
    if (m_clickCallback == nullptr || (m_clickCallbackName != m_prevClickCallbackName)) {
        if (AvailableActionsClick.find(m_clickCallbackName) != AvailableActionsClick.end()) {
            m_clickCallback = AvailableActionsClick[m_clickCallbackName];
        }
        m_prevClickCallbackName = m_clickCallbackName;
    }
    TestClick();
    Sys.GetGraphicalModule()->WindowDrawText(
        {
            .pos = {m_uiDiv->GetPosition().x + 5, m_uiDiv->GetPosition().y + 2},
            .text = m_text,
            .color = {255, 255, 255, 255},
            .fontSize = 12
        }
    );
}

void UIButton::OnAddComponent(int entityID)
{
    UIDiv *uiDiv = nullptr;

    try {
        uiDiv = &(Sys.GetComponent<UIDiv>(entityID));
    } catch (std::exception &e) {
        Sys.AddComponent<UIDiv>(entityID);
        uiDiv = &(Sys.GetComponent<UIDiv>(entityID));
    }
    m_uiDiv = uiDiv;
    m_hoverDiv = uiDiv;
    SetHoverCallback([&]{
        graph::vec2f mousePos = Sys.GetGraphicalModule()->WindowGetMousePos();
        Sys.GetGraphicalModule()->WindowDrawText({
            .pos = {mousePos.x + 10, mousePos.y + 10},
            .text = m_description,
            .color = {255, 255, 255, 255},
            .fontSize = 12
        });
    });
}

void UIButton::RegisterCallback(const std::string &name, std::function<void()> &&callback)
{
    AvailableActionsClick[name] = callback;
}

void UIButton::OnLoad()
{
    Entity systemHolder = Sys.GetSystemHolder();
    try {
        auto &cli = Sys.GetComponent<CLI>(systemHolder);
        cli.RegisterCustomCommand("bt-listcb", [&](CLI &c, std::vector<std::string> args) {
            Console::info << "Available callbacks:" << std::endl;
            for (auto &cb : AvailableActionsClick)
                Console::info << "\t - " << cb.first << std::endl;
        });
        cli.RegisterCustomCommand("bt-setcb", [&](CLI &c, std::vector<std::string> args) {
            Entity ctxt = c.GetContext();
            try {
                auto &bt = Sys.GetComponent<UIButton>(ctxt);
                bt.m_clickCallbackName = args[0];
            } catch (std::exception &e) {
                Console::warn << "No UIButton component on entity " << ctxt << std::endl;
            }
        });
    } catch (std::exception &e) {
        Console::warn << "No Cli, cannot change commands for UIButton" << std::endl;
    }
}
