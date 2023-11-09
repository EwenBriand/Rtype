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

            /**
             * @brief Enables the hoverable feature.
             *
             * @param hoverable
             */
            void SetHoverable(bool hoverable);

            /**
             * @brief Returns whether the object is hoverable or not.
             *
             * @return true
             * @return false
             */
            bool IsHoverable() const;

            /**
             * @brief Sets the div to which the hoverable feature is attached.
             *
             * @param div
             */
            void SetDiv(UIDiv *div);

            /**
             * @brief Sets the callback to be played when the object is hovered.
             *
             * @param div
             */
            void SetHoverCallback(std::function<void()> callback);

            /**
             * @brief Sets the callback to be played when the object is unhovered.
             *
             * @param div
             */
            void SetOnUnhoverCallback(std::function<void()> callback);

            /**
             * @brief Returns whether the object is hovered or not.
             *
             * @return true
             * @return false
             */
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

            /**
             * @brief Tests if the object has been clicked onto, and if so, plays
             * the callback attached to the object.
             *
             */
            void TestClick();

            /**
             * @brief Enables the clickable feature.
             *
             * @param clickable
             */
            void SetClickable(bool clickable);

            /**
             * @brief Returns whether the object is clickable or not.
             *
             * @return true
             * @return false
             */
            bool IsClickable() const;

            /**
             * @brief Sets the callback to be played when the object is clicked.
             *
             * @param div
             */
            void SetClickCallback(std::function<void()> callback);
        protected:
            bool m_clickable = true;
            std::function<void()> m_clickCallback = nullptr;
    };

} // namespace graph

/**
 * @brief A class to manage globally the state of the focus of the application.
 * For exemple if an object gets clicked on, it will be considered as being
 * focused by the user.
 *
 */
class Focusable {
    public:
        static bool *FocusedElement;
        static bool IsFocused(bool &target);
        static void Focus(bool &target);
        static void UnFocus(bool &target);
};

#endif /* A5A12554_586C_47D7_AB10_6C41218A8FAA */
