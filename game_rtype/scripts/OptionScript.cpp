/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** OptionScript.cpp
*/

#include "OptionScript.hpp"

MANAGED_RESOURCE(OptionScript)

OptionScript::~OptionScript()
{
    if (_uiHandle != -1)
        ui::UIManager::Get().RemoveGroup(_uiHandle);
    _uiHandle = -1;
}

void OptionScript::Start()
{
    try {
        _uiHandle = ui::UIManager::Get().AddGroup(
            ui::UIManager::Button {
                .text = "Back",
                .position = { 100, 100 },
                .fontSize = 70,
                .callback = [this]() { backToMenu(); } },

            ui::UIManager::Button {
                .text = "Up FPS",
                .position = { 100, 200 },
                .fontSize = 70,
                .callback = [this]() { upFpsUp(); } },

            ui::UIManager::Button {
                .text = "Down FPS",
                .position = { 100, 300 },
                .fontSize = 70,
                .callback = [this]() { upFpsDown(); } });
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void OptionScript::Update(int entityID)
{
}

void OptionScript::OnAddComponent(int e)
{
}

// ----------------------------------------------------------------------------
// Private methods
// ----------------------------------------------------------------------------

void OptionScript::backToMenu()
{
    if (_uiHandle != -1)
        ui::UIManager::Get().RemoveGroup(_uiHandle);
    _uiHandle = -1;
    eng::Engine::GetEngine()->GetSceneManager().SwitchScene("menu");
}

void OptionScript::upFpsDown()
{
    auto graphModule = eng::Engine::GetEngine()->GetGraphicalModule();
    auto fps = graphModule->GetMaxFPS();
    if (fps > 2)
        graphModule->SetMaxFPS(fps - 1);
}

void OptionScript::upFpsUp()
{
    auto graphModule = eng::Engine::GetEngine()->GetGraphicalModule();
    auto fps = graphModule->GetMaxFPS();
    graphModule->SetMaxFPS(fps + 1);
}