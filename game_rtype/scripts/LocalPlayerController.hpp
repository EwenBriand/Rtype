/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LocalPlayerController.hpp
*/

#pragma once
#include "./Ship.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IController.hpp"
#include "metadata.hpp"

serialize class LocalPlayerController : public AUserComponent, public ecs::AController {
public:
    GENERATE_METADATA(LocalPlayerController);

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

    LocalPlayerController() = default;
    ~LocalPlayerController();

    // ====================================================================
    // getters/setters
    // ====================================================================

private:
    // ====================================================================
    // private methods
    // ====================================================================

    bool testUp();
    bool testDown();
    bool testLeft();
    bool testRight();
    bool testShoot();

    // ====================================================================
    // properties
    // ====================================================================
    std::vector<std::string> _directives;

    std::map<std::string, bool (LocalPlayerController::*)()> _directivesTests = {
        { Ship::COMMAND_UP, &LocalPlayerController::testUp },
        { Ship::COMMAND_DOWN, &LocalPlayerController::testDown },
        { Ship::COMMAND_LEFT, &LocalPlayerController::testLeft },
        { Ship::COMMAND_RIGHT, &LocalPlayerController::testRight },
        { Ship::COMMAND_SHOOT, &LocalPlayerController::testShoot }
    };
};