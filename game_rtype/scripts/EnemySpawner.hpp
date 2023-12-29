/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** EnemySpawner.hpp
*/

#pragma once

#include "AIController.hpp"
#include "Component.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "GameRtype.hpp"
#include "IGraphicalModule.hpp"
#include "Timer.hpp"
#include "metadata.hpp"
#include <random>

BUILD_MANUAL
serialize class EnemySpawner : public AUserComponent {
public:
    GENERATE_METADATA(EnemySpawner)

    static unsigned int id;

    ~EnemySpawner();

    void Update(int e) override;
    void OnAddComponent(int e) override;
    void Start() override;
    void OnLoad() override;

private:
    void broadcastSpawn(const rtype::AIController& controller, int x, int y);
    void setupObserver(std::shared_ptr<rtype::AIController> controller, int e);

    serialize float _spawnDelay
        = 5.0f;
    serialize std::string _prefabName = "dual-ship";

    eng::Timer _timer;
    float _randomDelayVariation = 0.0f;
    float _maxRandomDelayVariation = 3.0f;
    CoreTransform* _transform = nullptr;
    std::mt19937 _rng;
};