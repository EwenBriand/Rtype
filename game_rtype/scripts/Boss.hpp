/*
** EPITECH PROJECT, 2023
** safe
** File description:
** Boss.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "metadata.hpp"
#include <map>

BUILD_MANUAL

serialize class Boss : public AUserComponent, public ecs::AActor {
public:
    GENERATE_METADATA(Boss)
    Boss() = default;
    ~Boss() = default;

    // ==============================================================
    // COMPONENT
    // ==============================================================

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    // ==============================================================
    // ACTOR
    // ==============================================================

    void Possess(int entity, std::shared_ptr<ecs::IController>) override;

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

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void shoot();

    // ==============================================================
    // ATTRIBUTES
    // ==============================================================

    std::map<std::string, void (Boss::*)()> _directivesCallbacks = {
        { "up", &Boss::moveUp },
        { "down", &Boss::moveDown },
        { "left", &Boss::moveLeft },
        { "right", &Boss::moveRight },
        { "shoot", &Boss::shoot },
    };
    int _entityID = -1;

    // ==============================================================
    // serialization
    // ==============================================================
    serialize float _speed = 10.0f;
    serialize float _hp = 100.0f;
    serialize int _bodyParts = 4;
};