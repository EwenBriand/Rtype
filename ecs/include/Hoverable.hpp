/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Hoverable.hpp
*/

#ifndef A5A12554_586C_47D7_AB10_6C41218A8FAA
#define A5A12554_586C_47D7_AB10_6C41218A8FAA
#include <functional>
#include <iostream>
#include "IGraphicalModule.hpp"
#include "Components.Vanilla/UIDiv.hpp"

namespace graph
{
    /**
     * @brief Base class for objects that can be hovered.
     *
     */
    class Hoverable {
        public:
            void TestHover();

            void SetHoverable(bool hoverable);
            bool IsHoverable() const;

            void SetDiv(UIDiv *div);
            void SetHoverCallback(std::function<void()> callback);
            void SetOnUnhoverCallback(std::function<void()> callback);
            bool IsHovered();
        protected:
            bool m_hoverable = true;
            bool m_isHovered = false;
            std::function<void()> m_callback = nullptr;
            std::function<void()> m_onUnhoverCallback = nullptr;
            UIDiv *m_hoverDiv = nullptr;
    };

    /**
     * @brief Base class for objects that can be clicked.
     *
     */
    class Clickable : public Hoverable {
        public:
            void TestClick();
            void SetClickable(bool clickable);
            bool IsClickable() const;

            void SetClickCallback(std::function<void()> callback);
        protected:
            bool m_clickable = true;
            std::function<void()> m_clickCallback = nullptr;
    };

} // namespace graph

class Focusable {
    public:
        static bool *FocusedElement;
        static bool IsFocused(bool &target);
        static void Focus(bool &target);
        static void UnFocus(bool &target);
};

#endif /* A5A12554_586C_47D7_AB10_6C41218A8FAA */
