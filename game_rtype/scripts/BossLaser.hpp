/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossLaser.hpp
*/

#pragma once
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "metadata.hpp"

BUILD_MANUAL

serialize class BossLaser : public AUserComponent {
public:
    GENERATE_METADATA(BossLaser)
    BossLaser() = default;
    ~BossLaser() = default;

    // ==============================================================
    // COMPONENT
    // ==============================================================

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    // ==============================================================
    // GETTERS & SETTERS
    // ==============================================================
    void SetDirection(graph::vec2f direction);
    // ==============================================================
    // PUBLIC METHODS
    // ==============================================================
private:
    // ==============================================================
    // PRIVATE METHODS
    // ==============================================================
    void move();
    bool destroyIfOutOfScreen();
    // ==============================================================
    // ATTRIBUTES
    // ==============================================================
    graph::vec2f _direction = { 1, 1 };
    float _speed = 60;
    int _entityID = -1;
};
