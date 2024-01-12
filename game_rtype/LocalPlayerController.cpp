/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LocalPlayerController.cpp
*/

#include "LocalPlayerController.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "GameRtype.hpp"
#include "scripts/Ship.hpp"

// ====================================================================
// IController methods
// ====================================================================

LocalPlayerController::LocalPlayerController()
{
    _shootTimer.Start();
}

std::vector<std::string>& LocalPlayerController::GetDirectives()
{
    return _directives;
}

void LocalPlayerController::PollDirectives()
{
    _directives.clear();
    for (auto& [directive, test] : _directivesTests) {
        if ((this->*test)()) {
            _directives.push_back(directive);
        }
    }
}

// ====================================================================
// public methods
// ====================================================================

// ====================================================================
// getters/setters
// ====================================================================

// ====================================================================
// private methods
// ====================================================================

bool LocalPlayerController::testUp()
{
    return SYS.GetInputManager().isDown(KEY_UP);
}

bool LocalPlayerController::testDown()
{
    return SYS.GetInputManager().isDown(KEY_DOWN);
}

bool LocalPlayerController::testLeft()
{
    return SYS.GetInputManager().isDown(KEY_LEFT);
}

bool LocalPlayerController::testRight()
{
    return SYS.GetInputManager().isDown(KEY_RIGHT);
}

bool LocalPlayerController::testShoot()
{
    bool shot = SYS.GetInputManager().MouseButtonPressed(MOUSE_BUTTON_LEFT);
    auto* engine = eng::Engine::GetEngine();

    if (shot and engine->IsClient()) {
        try {
            auto& transform = SYS.GetComponent<CoreTransform>(_entity);
            std::vector<int> data = {
                _playerId,
                static_cast<int>(transform.x),
                static_cast<int>(transform.y)
            };
            // std::cout << "\rplayer " << _playerId << " shoots at " << transform.x << ", " << transform.y << std::endl;
            engine->GetClient().Send(serv::Instruction(eng::RType::I_PLAYER_SHOOTS, 0, serv::bytes(data)));
        } catch (const std::exception& e) {
            CONSOLE::err << "\rFailed to send shoot instruction to server." << std::endl;
        };
    }
    return shot;
}

bool LocalPlayerController::testTcemort()
{
    if (_shootTimer.GetElapsedTime() > _shootInterval) {
        _shootTimer.Restart();
        return true;
    }
    return false;
}

bool LocalPlayerController::testLaunch()
{
    return SYS.GetInputManager().MouseButtonPressed(MOUSE_BUTTON_RIGHT);
}
