/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** MoveBoat.hpp
*/

#pragma once

#include <ctime>
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

serialize class MoveBoat : public AUserComponent {
    public:
        GENERATE_METADATA(MoveBoat)
        MoveBoat();

        void Update(int entityID) override;
        void Start() override;

    private:
        clock_t t;
        int m_amplitude = 1;
        serialize float m_speed = 0.1f;
        serialize float m_rotationSpeed = 0.1f;
};
