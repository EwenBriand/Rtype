
/*
** @file MenuScript.cpp
** @brief Implementation of the MenuScript class.
** @date 2023-12-14 11:30:57.
**
*/

#include "MenuScript.hpp"
#include "Engine.hpp"
#include "RTypeDistantServer.hpp"
#include "UIManager.hpp"
#include <iostream>

MANAGED_RESOURCE(MenuScript)

bool MenuScript::_callbacksRegistered = false;

MenuScript::~MenuScript()
{
    if (_menuUIHandle != -1) {
        ui::UIManager::Get().RemoveGroup(_menuUIHandle);
        _menuUIHandle = -1;
    }
}

void MenuScript::Start()
{
    if (_callbacksRegistered)
        return;
    _callbacksRegistered = true;
    registerButtonCallbacks();
}

void MenuScript::Update(int entityID)
{
    if (_menuUIHandle != -1)
        return;
    try {
        _menuUIHandle = ui::UIManager::Get().AddGroup(
            ui::UIManager::Button {
                .text = "Play",
                .position = { 100, 100 },
                .fontSize = 70,
                .callback = [this]() {
                    std::cout << "before remove group" << std::endl;
                    ui::UIManager::Get().RemoveGroup(_menuUIHandle);
                    _menuUIHandle = -1;
                    std::cout << "before switch scene" << std::endl;
                    eng::Engine::GetEngine()->GetSceneManager().SwitchScene("lobby");
                    std::cout << "callback done" << std::endl;
                } });
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void MenuScript::OnAddComponent(int e)
{
}

void MenuScript::registerButtonCallbacks()
{
    UIButton::RegisterCallback("ui::backToMenu", [this]() {
        auto engine = eng::Engine::GetEngine();
        if (engine->IsClient()) {
            engine->GetClient().Send(serv::Instruction(serv::I_DISCONNECT, 0, serv::bytes()));
        }
    });
}