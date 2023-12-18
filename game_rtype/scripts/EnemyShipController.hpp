/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** EnemyShipController.hpp
*/

#pragma once
#include "./Ship.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IController.hpp"
#include "metadata.hpp"

serialize class EnemyShipController : public AUserComponent, public ecs::AController {
public:
    GENERATE_METADATA(EnemyShipController);

    // ====================================================================
    // Component methods
    // ====================================================================

    void Start() override;
    void Update(int entityID) override;
    void OnAddComponent(int entityID) override;

    // ====================================================================
    // IController methods
    // ====================================================================

    std::vector<std::string>& GetDirectives() override;
    void PollDirectives() override;

    // ====================================================================
    // public methods
    // ====================================================================

    EnemyShipController() = default;
    ~EnemyShipController() = default;

    // ====================================================================
    // getters/setters
    // ====================================================================

private:
    // ====================================================================
    // private methods
    // ====================================================================
    // ====================================================================
    // properties
    // ====================================================================
    std::vector<std::string> _directives;
};