/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Hoverable.cpp
*/

#include "ECSImpl.hpp"
#include "Hoverable.hpp"

namespace graph
{
    void Hoverable::TestHover()
    {
        if (!m_hoverable || !m_hoverDiv)
            return;

        vec2f mousePos = Sys.GetGraphicalModule()->WindowGetMousePos();
        vec2f divPos = m_hoverDiv->GetPosition();
        vec2f divSize = m_hoverDiv->GetSize();

        if (mousePos.x >= divPos.x && mousePos.x <= divPos.x + divSize.x &&
            mousePos.y >= divPos.y && mousePos.y <= divPos.y + divSize.y) {
            m_isHovered = true;
            if (m_callback)
            m_callback();
        } else {
            if (m_isHovered && m_onUnhoverCallback)
                m_onUnhoverCallback();
            m_isHovered = false;
        }
    }

    void Hoverable::SetOnUnhoverCallback(std::function<void()> callback)
    {
        m_onUnhoverCallback = callback;
    }

    bool Hoverable::IsHovered()
    {
        TestHover();
        return m_isHovered;
    }

    void Hoverable::SetHoverable(bool hoverable)
    {
        m_hoverable = hoverable;
    }

    bool Hoverable::IsHoverable() const
    {
        return m_hoverable;
    }

    void Hoverable::SetHoverCallback(std::function<void()> callback)
    {
        m_callback = callback;
    }

    void Hoverable::SetDiv(UIDiv *div)
    {
        m_hoverDiv = div;
    }

    // Clickable

    void Clickable::TestClick()
    {
        if (IsHovered() && m_clickable && m_clickCallback &&
            Sys.GetGraphicalModule()->WindowIsMouseLeftPressed()) {
            m_clickCallback();
        }
    }

    void Clickable::SetClickable(bool clickable)
    {
        m_clickable = clickable;
    }

    bool Clickable::IsClickable() const
    {
        return m_clickable;
    }

    void Clickable::SetClickCallback(std::function<void()> callback)
    {
        m_clickCallback = callback;
    }

} // namespace graph

// Focusable
bool *Focusable::FocusedElement = nullptr;

void Focusable::Focus(bool &target)
{
    if (FocusedElement) {
        UnFocus(*FocusedElement);
    }
    FocusedElement = &target;
    target = true;
}

void Focusable::UnFocus(bool &target)
{
    if (FocusedElement)
        *FocusedElement = false;
    FocusedElement = nullptr;
}

bool Focusable::IsFocused(bool &target)
{
    return (FocusedElement != nullptr) and (FocusedElement == &target);
}

