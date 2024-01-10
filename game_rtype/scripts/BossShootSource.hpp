/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossShootSource.hpp
*/

#pragma once
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "Timer.hpp"
#include "metadata.hpp"

BUILD_MANUAL

serialize class BossShootSource : public AUserComponent {
public:
    GENERATE_METADATA(BossShootSource)
    static int Ids;
    BossShootSource();
    ~BossShootSource() = default;

    // ==============================================================
    // COMPONENT
    // ==============================================================

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    // ==============================================================
    // GETTERS & SETTERS
    // ==============================================================

    // ==============================================================
    // PUBLIC METHODS
    // ==============================================================
private:
    // ==============================================================
    // PRIVATE METHODS
    // ==============================================================
    void shoot();
    // ==============================================================
    // ATTRIBUTES
    // ==============================================================
    int _entityID = -1;
    int _id = 0;
    eng::Timer _shootTimer;
};