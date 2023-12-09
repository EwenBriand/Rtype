/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIMenu.hpp
*/

#pragma once

#include "../Component.hpp"
#include "../Hoverable.hpp"
#include "metadata.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

needs(-luuid)

    /**
     * @brief A Scrollable menu. Can contain various entries, as buttons, checkboxes
     * or text input fields.
     *
     */
    serialize class UIMenu : public ecs::Component<UIMenu>,
                             public graph::Hoverable {
public:
    GENERATE_METADATA(UIMenu)

    void Update(int entityID);
    void OnAddComponent(int entityID);

    void SetAlwaysVisible(bool alwaysVisible);

private:
    serialize float m_x;
    serialize float m_y;

    serialize bool m_alwaysVisible = true;
    bool m_isVisible = false;
};
