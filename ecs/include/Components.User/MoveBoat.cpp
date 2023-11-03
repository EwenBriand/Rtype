/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** MoveBoat.cpp
*/

#include <random>
#include <cmath>
#include <ECSImpl.hpp>
#include "Components.Vanilla/CoreTransform.hpp"
#include "MoveBoat.hpp"

MANAGED_RESOURCE(MoveBoat)

MoveBoat::MoveBoat() : t(clock())
{
    m_amplitude = rand() % 1000;
}

void MoveBoat::Update(int entityID) {
    try {
        auto &coreTransform = Sys.GetComponent<CoreTransform>(entityID);
        auto deltaTime = (float)(clock() - t) / CLOCKS_PER_SEC;
        t = clock();
        coreTransform.y = sin(deltaTime * m_speed) * m_amplitude;
    } catch (std::exception) {
        std::cout << "Error: CoreTransform not found\n" << std::endl;
        return;
    }
}

void MoveBoat::Start() {}
