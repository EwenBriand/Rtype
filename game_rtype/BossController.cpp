/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossController.cpp
*/

#include "BossController.hpp"
#include "GameRtype.hpp"

namespace rtype {
    BossController::BossController()
    {
        _shootTimer.Start();
        _generator = std::mt19937(_distribution(_generator));
    }

    void BossController::PollDirectives()
    {
        _directives.clear();
        if (not eng::Engine::GetEngine()->PlayMode())
            return;
        try {
            auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
            _currentPosition = { static_cast<int>(transform.x), static_cast<int>(transform.y) };
            getTargetPosition();
            for (auto& [directive, test] : _directivesTests) {
                if ((this->*test)())
                    _directives.push_back(directive);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void BossController::SetEntityID(int entityID)
    {
        _entityID = entityID;
    }

    void BossController::getTargetPosition()
    {
        if (eng::Engine::GetEngine()->IsClient()) {
            try {
                _targetPosition = eng::Engine::GetEngine()->GetGlobal<graph::vec2i>("bossTargetPosition");
            } catch (const std::exception& e) {
                return;
            }
            return;
        }

        // only in server mode from here on
        bool targetPosChanged = false;
        if ((_currentPosition - _targetPosition).magnitude() > 20)
            return;
        while ((_currentPosition - _targetPosition).magnitude() < 300) {
            _targetPosition.x = _distribution(_generator) % 1920; // todo : get screen size -> implement in graphical module
            _targetPosition.y = _distribution(_generator) % 1080;
            targetPosChanged = true;
        }
        if (targetPosChanged)
            broadcastNewTargetPosition();
    }

    void BossController::broadcastNewTargetPosition()
    {
        if (not eng::Engine::GetEngine()->IsServer())
            return;
        auto& server = eng::Engine::GetEngine()->GetServer();
        server.Broadcast(serv::Instruction(eng::RType::I_BOSS_MOVES, 0, serv::bytes(std::vector<int> { _targetPosition.x, _targetPosition.y })));
    }

    std::vector<std::string>& BossController::GetDirectives()
    {
        return _directives;
    }

    bool BossController::moveUp()
    {
        if (_currentPosition.y - _targetPosition.y > 5) {
            return true;
        }
        return false;
    }

    bool BossController::moveDown()
    {
        if (_targetPosition.y - _currentPosition.y > 5) {
            return true;
        }
        return false;
    }

    bool BossController::moveLeft()
    {
        if (_currentPosition.x - _targetPosition.x > 5) {
            return true;
        }
        return false;
    }

    bool BossController::moveRight()
    {
        if (_targetPosition.x - _currentPosition.x > 5) {
            return true;
        }
        return false;
    }

    bool BossController::shoot()
    {
        if (_shootTimer.GetElapsedTime() > 2) {
            _shootTimer.Restart();
            return true;
        }
        return false;
    }
}