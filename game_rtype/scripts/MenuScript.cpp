
/*
** @file MenuScript.cpp
** @brief Implementation of the MenuScript class.
** @date 2023-12-14 11:30:57.
**
*/

#include "MenuScript.hpp"
#include "Engine.hpp"
#include "UIManager.hpp"
#include <iostream>

MANAGED_RESOURCE(MenuScript)

MenuScript::~MenuScript()
{
    if (_menuUIHandle != -1)
        ui::UIManager::Get().RemoveGroup(_menuUIHandle);
}

void MenuScript::Start()
{
}

void MenuScript::Update(int entityID)
{
}

void MenuScript::OnAddComponent(int e)
{
    try {
        _menuUIHandle = ui::UIManager::Get().AddGroup(
            ui::UIManager::Button {
                .text = "Play",
                .position = { 100, 100 },
                .fontSize = 70,
                .callback = [this]() {
                    ui::UIManager::Get().RemoveGroup(_menuUIHandle);
                    _menuUIHandle = -1;
                    eng::Engine::GetEngine()->GetSceneManager().SwitchScene("level1");
                } });
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
