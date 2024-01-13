/*
** EPITECH PROJECT, 2023
** safe
** File description:
** Car.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "metadata.hpp"

BUILD_MANUAL

/**
 * @brief A script to control a small 3D car in an arcade style.
 *
 */
serialize class Car : public AUserComponent, public ecs::AActor {
public:
    GENERATE_METADATA(Car)
    Car() = default;
    ~Car() = default;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void SetID(int id);
    int GetID() const;

private:
    void correctAltitude();
    void correctMapBounds();

    void commandLeft();
    void commandRight();
    void commandForward();
    void commandBackward();
    void commandBrake();

    serialize float _maxSpeed = 10.0f;
    serialize float _acceleration = 0.5f;
    serialize float _deceleration = 5.0f;
    serialize float _rotationSpeed = 0.5f;

    int _parentID;
    bool _isTurning = false;

    /**
     * @brief Applies the various directives to the car.
     *
     */
    void applyDirectives();

    std::map<std::string, void (Car::*)()> _commands = {
        { "left", &Car::commandLeft },
        { "right", &Car::commandRight },
        { "forward", &Car::commandForward },
        { "backward", &Car::commandBackward },
        { "brake", &Car::commandBrake },
    };
};
