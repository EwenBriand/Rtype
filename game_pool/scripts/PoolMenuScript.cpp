
/*
** @file PoolMenuScript.cpp
** @brief Implementation of the PoolMenuScript class.
** @date 2023-12-14 11:30:57.
**
*/

#include "PoolMenuScript.hpp"
#include "Engine.hpp"
#include "NetworkExceptions.hpp"
#include "UIManager.hpp"
#include <iostream>

MANAGED_RESOURCE(PoolMenuScript)

bool PoolMenuScript::_callbacksRegistered = false;

PoolMenuScript::~PoolMenuScript()
{
    if (_menuUIHandle != -1) {
        ui::UIManager::Get().RemoveGroup(_menuUIHandle);
        _menuUIHandle = -1;
    }
}

void PoolMenuScript::Start()
{
    if (_callbacksRegistered)
        return;
    _callbacksRegistered = true;
    registerButtonCallbacks();
}

void PoolMenuScript::Update(int entityID)
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
                    ui::UIManager::Get().RemoveGroup(_menuUIHandle);
                    _menuUIHandle = -1;
                    eng::Engine::GetEngine()->GetSceneManager().SwitchScene("lobby");
                } });
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void PoolMenuScript::OnAddComponent(int e)
{
}

void PoolMenuScript::registerButtonCallbacks()
{
    UIButton::RegisterCallback("ui::backToMenu", [this]() {
        auto engine = eng::Engine::GetEngine();
        if (engine->IsClient()) {
            engine->GetClient().Send(serv::Instruction(serv::I_DISCONNECT, 0, serv::bytes()));
        }
    });
}