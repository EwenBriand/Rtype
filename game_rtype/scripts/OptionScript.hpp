/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** OptionScript.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "UIManager.hpp"
#include "metadata.hpp"

serialize class OptionScript : public AUserComponent {
public:
    GENERATE_METADATA(OptionScript)
    OptionScript() = default;
    ~OptionScript();

    void Start() override;
    void Update(int entityID) override;
    void OnAddComponent(int entityID) override;

private:
    void upFpsUp();
    void upFpsDown();
    void backToMenu();

    int _uiHandle = -1;
};