/*
** EPITECH PROJECT, 2023
** safe
** File description:
** PoolPoolMenuScript.hpp
*/

#pragma once

#include "Components.Vanilla/UIButton.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

serialize class PoolMenuScript : public AUserComponent {
public:
    GENERATE_METADATA(PoolMenuScript)
    PoolMenuScript() = default;
    ~PoolMenuScript();

    void Start() override;
    void Update(int entityID) override;
    void OnAddComponent(int entityID) override;

    static bool _callbacksRegistered;

private:
    void registerButtonCallbacks();

    std::size_t _menuUIHandle = -1;
};
