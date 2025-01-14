/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** TextField.cpp
*/

#include "TextField.hpp"
#include "../ECSImpl.hpp"
#include "../Hoverable.hpp"
#include "../IGraphicalModule.hpp"
#include <algorithm>
#include <functional>

void TextField::Update(int entityID)
{
    TestClick();
    if (Focusable::IsFocused(m_isFocused)) {
        collectInput();
    }
    Draw();
}

void TextField::SetParent(UIDiv& parent)
{
    m_hoverDiv = &parent;
}

void TextField::Draw()
{
    std::shared_ptr<graph::IGraphicalModule> graphics = SYS.GetGraphicalModule();
    graph::vec2f divPos = m_hoverDiv->GetPosition();

    graph::graphRect_t rectInfo = { .pos = { divPos.x + m_x + 11.0f * m_label.size(), divPos.y + m_y },
        .dimensions = _dimension,
        .bgColor = { 255, 255, 255, 255 },
        .borderColor = { 255, 255, 255, 255 },
        .borderSize = 0 };

    graph::graphText_t textInfo = { .pos = { divPos.x + m_x, divPos.y + m_y },
        .text = m_label,
        .color = { 255, 255, 255, 255 },
        .fontSize = 11 };

    graphics->AddRectToBuffer(1, [rectInfo]() {
        DrawRectangle(rectInfo.pos.x, rectInfo.pos.y, rectInfo.dimensions.x, rectInfo.dimensions.y, { rectInfo.bgColor.x, rectInfo.bgColor.y, rectInfo.bgColor.z, rectInfo.bgColor.w });
        DrawRectangleLinesEx({ rectInfo.pos.x, rectInfo.pos.y, rectInfo.dimensions.x, rectInfo.dimensions.y }, rectInfo.borderSize, { rectInfo.borderColor.x, rectInfo.borderColor.y, rectInfo.borderColor.z, 255 });
    });
    graphics->AddRectToBuffer(1, [textInfo]() {
        DrawText(textInfo.text.c_str(), textInfo.pos.x, textInfo.pos.y, textInfo.fontSize, { textInfo.color.x, textInfo.color.y, textInfo.color.z, textInfo.color.w });
    });

    size_t start = 0;
    if (m_text.size() - 20 > 0)
        start = m_text.size();
    std::string text;
    if (m_text == "")
        text = m_placeholder;
    else
        text = m_text.substr((m_text.size() > 20) ? (m_text.size() - 20) : 0, m_text.size());

    graph::graphText_t textInfo2 = {
        .pos = { divPos.x + m_x + 11.0f * m_label.size() + 5, divPos.y + m_y },
        .text = text,
        .color = (m_text == "") ? (graph::vec4uc) { 100, 100, 100, 255 } : (graph::vec4uc) { 0, 0, 0, 255 },
        .fontSize = 11
    };
    graphics->AddRectToBuffer(1, [textInfo2]() {
        DrawText(textInfo2.text.c_str(), textInfo2.pos.x, textInfo2.pos.y, textInfo2.fontSize, { textInfo2.color.x, textInfo2.color.y, textInfo2.color.z, textInfo2.color.w });
    });
}

void TextField::SetupCallbacks()
{
    auto graphics = SYS.GetGraphicalModule().get();
    if (graphics == nullptr)
        throw std::runtime_error("No graphical module found");

    SYS.GetInputManager().RegisterBinding(
        "key code " + std::to_string(KEY_BACKSPACE),
        { .testTriggered = [&](InputManager::EventInfo& info) { return SYS.GetGraphicalModule()->isKeyPressed(KEY_BACKSPACE); },
            .onTriggerCallback = [&](InputManager::EventInfo info) {
                if (m_text.size() > 0) {
                    m_text.pop_back();
                } } });

    SYS.GetInputManager().RegisterBinding(
        "key code " + std::to_string(KEY_ENTER),
        { .testTriggered = [&](InputManager::EventInfo& info) {
             return SYS.GetGraphicalModule()->isKeyPressed(KEY_ENTER);
         },
            .onTriggerCallback = nullptr });

    SetClickCallback([&]() {
        Focusable::Focus(m_isFocused);
    });
}

void TextField::OnAddComponent(int entityID)
{
    UIDiv* div = nullptr;
    try {
        div = &SYS.GetComponent<UIDiv>(entityID);
    } catch (std::exception& e) {
        SYS.AddComponent<UIDiv>(entityID);
        div = &SYS.GetComponent<UIDiv>(entityID);
    }
    m_hoverDiv = div;
    SetupCallbacks();
}

void TextField::RegisterOnEnterCallback(std::function<void()> callback)
{
    m_onEnterCallback = callback;
}

void TextField::SetPosition(graph::vec2f pos)
{
    m_x = pos.x;
    m_y = pos.y;
}

// need to remake it with the new input manager
void TextField::collectInput()
{
    auto inputManager = SYS.GetInputManager();
    std::vector<std::shared_ptr<InputManager::EventInfo>> inputs = inputManager.GetPolledEvents();

    for (size_t i = 0; i < inputs.size(); ++i) {
        if (inputs[i]->key_code == KEY_ENTER)
            std::cout << "enter has been pressed" << std::endl;
        auto evtName = inputs[i]->key_code;
        if (inputs[i]->type == inputManager.KEYBOARD && evtName == KEY_BACKSPACE && inputManager.isDown(inputManager.KeyCodeTOName(evtName))) {
            if (m_text.size() > 0) {
                m_text.pop_back();
            }
        // } else if (inputs[i]->type == inputManager.KEYBOARD && evtName == KEY_ENTER && inputManager.isReleased(inputManager.KeyCodeTOName(evtName))) {
        } else if (inputs[i]->key_code == KEY_ENTER) {
            std::cout << "in textfield, enter was pressed" << std::endl;
            if (m_onEnterCallback) {
                m_onEnterCallback();
            } else CONSOLE::warn << "no callback set to textfield" << std::endl;
            Focusable::UnFocus(m_isFocused);
        } else if (inputs[i]->type == inputManager.KEYBOARD && inputManager.isReleased(inputManager.KeyCodeTOName(evtName))) {
            std::cout << "text: " << m_text << std::endl;
            m_text += inputManager.GetLastCharPressed();
        }
    }
}

void TextField::SetSize(graph::vec2f dim)
{
    _dimension = dim;
}

void TextField::SetLabel(std::string label)
{
    m_label = label;
}

void TextField::SetPlaceholder(std::string placeholder)
{
    m_placeholder = placeholder;
}

void TextField::SetText(std::string text)
{
    m_text = text;
}

std::string TextField::GetText() const
{
    std::cout << "TextField::text: " << m_text << std::endl;
    return m_text;
}

bool TextField::IsFocused() const
{
    return m_isFocused;
}

void TextField::Focus()
{
    Focusable::Focus(m_isFocused);
}

void TextField::UnFocus()
{
    Focusable::UnFocus(m_isFocused);
}
