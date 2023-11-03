/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Animator.cpp
*/

#include "ECSImpl.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "Components.Vanilla/Animator.hpp"
#include "Engine.hpp"

void Animator::OnLoad() {
    if (!eng::Engine::GetEngine()->IsOptionSet("--editor"))
        return;
    Entity e = Sys.GetSystemHolder();
    CLI &cli = Sys.GetComponent<CLI>(e);

    cli.RegisterCustomCommand("addanim", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
        if (args.size() < 2)
            return;
        _registeredAnimations.push_back(args[1]);
        // TODO LOAD THE ANIMATION FROM BINARY
    });

    cli.RegisterCustomCommand("newanim", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
        if (args.size() < 2)
            return;
        // todo show animation editor
    });
}
