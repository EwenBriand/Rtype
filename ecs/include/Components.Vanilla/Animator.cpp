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
#include "UIButton.hpp"

void Animator::editorRegisterUIButtonExposed() {
    UIButton::RegisterCallback("animeditor::saveCurrAnim", [&] {
        saveCurrAnim();
    });
}

void Animator::saveCurrAnim() {
    if (_animEditorAnimationName == "") {
        Console::warn << "Couldn't save the animation because it has no name." << std::endl;
        return;
    }


}

void Animator::OnLoad() {
    if (!eng::Engine::GetEngine()->IsOptionSet("--editor"))
        return;
    Entity e = Sys.GetSystemHolder();
    CLI &cli = Sys.GetComponent<CLI>(e);

    editorRegisterUIButtonExposed();

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
