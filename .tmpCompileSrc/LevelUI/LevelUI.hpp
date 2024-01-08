/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LevelUI.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

/**
 * @brief displays buttons to pause the game and to quit the game
 *
 */
serialize class LevelUI : public AUserComponent {
public:
    GENERATE_METADATA(LevelUI)
    LevelUI() = default;
    ~LevelUI() = default;

    void Start() override;
    void Update(int e) override;

private:
};