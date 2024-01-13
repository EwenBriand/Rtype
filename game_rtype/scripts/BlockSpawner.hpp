/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** BlockSpawner.hpp
*/

#pragma once

#include "AIController.hpp"
#include "Block.hpp"
#include "Component.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "GameRtype.hpp"
#include "IGraphicalModule.hpp"
#include "Timer.hpp"
#include "metadata.hpp"
#include <random>

BUILD_MANUAL

serialize class BlockSpawner : public AUserComponent {
public:
    GENERATE_METADATA(BlockSpawner)

    static unsigned int id;

    ~BlockSpawner();

    void Update(int e) override;
    void OnAddComponent(int e) override;
    void Start() override;
    void OnLoad() override;

private:
    void broadcastSpawn(int x, int y);
    // void setupObserver(std::shared_ptr<rtype::AIController> controller, int e);
    void spawnBlock();

    serialize float _spawnDelay
        = 5.0f;
    serialize std::string _prefabName = "Block";

    eng::Timer _timer;
    float _randomDelayVariation = 0.0f;
    float _maxRandomDelayVariation = 3.0f;
    CoreTransform* _transform = nullptr;
    std::mt19937 _rng;
    std::vector<int> _wave_enemy = { 10, 20, 30, 40, 50 };
    int _wave = 0;
};
