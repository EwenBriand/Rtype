/*
 ** EPITECH PROJECT, 2023
 ** safe
 ** File description:
 ** Ball.cpp
 */

#pragma once
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"

/**
 * @brief The ball of the Pool game (which really is a rocket league game)
 *
 */
serialize class Ball : public AUserComponent {
public:
    GENERATE_METADATA(Ball)

    ~Ball();

    void Update(int e) override;
    void OnAddComponent(int e) override;
    void Start() override;
    void OnLoad() override;

private:
    void bounce();
    void setupCollider();
    void doCollisionPhysics(int thisE, int otherE);
    void checkMapBoundariesBounce();

    graph::vec3f _prevPos;
    int _entityID;
    bool _isSetup = false;
};