/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** TextField.hpp
*/

#pragma once
#include "../Component.hpp"
#include "../Hoverable.hpp"
#include "../IGraphicalModule.hpp"
#include "../InputManager.hpp"
#include "UIMenu.hpp"
#include "metadata.hpp"
#include <functional>
#include <string>

/**
 * @brief A Text Field that will execute a function when Enter is pressed while it is focused.
 * Upon being focused (clicked on) it will register itself as the focused menu in the
 * UIMenu class and unfocus all other menus.
 *
 */
serialize class TextField : public ecs::Component<TextField>, public graph::Clickable {
public:
    GENERATE_METADATA(TextField)

    /**
     * @brief Draws the input field and, if focused, reads the inputs from the
     * input manager in order to update its text.
     *
     * @param entityID
     */
    void Update(int entityID);

    /**
     * @brief If no div is present on the Entity, adds one. Then, registers the
     * div as its parent.
     * Sets the callback to enable focus on click.
     *
     * @param entityID
     */
    void OnAddComponent(int entityID);

    /**
     * @brief Draws the input field on the screen.
     *
     */
    void Draw();

    /**
     * @brief Sets the xy position of the input field on screen.
     * The position is relative to the parent div.
     *
     */
    void SetPosition(graph::vec2f pos);

    /**
     * @brief Sets the callback to execute when enter is pressed / the field gets
     * unfocused.
     *
     */
    void RegisterOnEnterCallback(std::function<void()>);

    /**
     * @brief Sets the div that will be the parent of the input field.
     *
     */
    void SetParent(UIDiv& div);

    /**
     * @brief Sets the label of the input field.
     *
     */
    void SetLabel(std::string label);

    /**
     * @brief Sets the placeholder of the input field.
     *
     */
    void SetPlaceholder(std::string placeholder);

    /**
     * @brief Sets the text of the input field.
     *
     */
    void SetText(std::string text);

    /**
     * @brief This function sets up the callbacks for the click and backspace
     * in order to focus the input field and delete the last character.
     * This function is public so that components that use a textfield without adding
     * it as a component can still use it. It is otherwise called in
     * OnAddComponent.
     *
     */
    void SetupCallbacks();

    /**
     * @brief Returns the text of the input field.
     *
     */
    std::string GetText() const;

    /**
     * @brief Returns true if the text field is actively focused by
     * the user.
     *
     */
    bool IsFocused() const;

    /**
     * @brief Focus the input field.
     *
     */
    void Focus();

    /**
     * @brief Unfocus the input field.
     *
     */
    void UnFocus();

private:
    void collectInput();

    serialize std::string m_label;
    serialize std::string m_placeholder;

    serialize float m_x;
    serialize float m_y;

    bool m_isFocused = false;
    std::string m_text;
    std::function<void()> m_onEnterCallback;
};
