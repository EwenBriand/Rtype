/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AIController.cpp
*/

#include "AIController.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"
#include "Observer.hpp"

namespace rtype {
    AIController::AIController()
    {
        srand(time(NULL));
        _shootTimer.Restart();
        _UpTimer.Restart();
        _DownTimer.Restart();
    }

    std::vector<std::string>& AIController::GetDirectives()
    {
        try {
            const auto& transform = SYS.GetComponent<CoreTransform>(_entity);
            if (transform.x < -100) {
                eng::Engine::GetEngine()->UnregisterObserver(_observer);
                SYS.UnregisterEntity(_entity);
            }
        } catch (const std::exception& e) {
            // entity was destroyed, will be cleaned up at the end of the frame
        }
        return _directives;
    }

    void AIController::PollDirectives()
    {
        _directives.clear();
        for (auto& [directive, test] : _directivesTests) {
            if ((this->*test)())
                _directives.push_back(directive);
        }
        broadcastPosition();
    }

    bool AIController::testLeft()
    {
        return true;
    }

    bool AIController::testUp()
    {
        // if (_UpTimer.GetElapsedTime() > _directivesInterval) {
        //     _UpTimer.Restart();
        //     return rand() % 2;
        // }
        return false;
    }

    bool AIController::testDown()
    {
        // if (_DownTimer.GetElapsedTime() > _directivesInterval) {
        //     _DownTimer.Restart();
        //     return rand() % 2;
        // }
        return false;
    }

    bool AIController::testShoot()
    {
        if (not eng::Engine::GetEngine()->IsServer()) // order to shoot comes from server for synchronization
            return false;
        if (_shootTimer.GetElapsedTime() > _shootInterval) {
            _shootTimer.Restart();
            broadcastShoot();
            instantiateRedLaser();
            return false;
        }
        return false;
    }

    void AIController::SetID(int id)
    {
        _id = id;
    }

    int AIController::GetID() const
    {
        return _id;
    }

    void AIController::SetObserver(std::shared_ptr<eng::Observer> observer)
    {
        _observer = observer;
    }

    void AIController::broadcastPosition()
    {
        if (eng::Engine::GetEngine()->IsClient() or _broadcastTimer.GetElapsedTime() < 0.5f) {
            return;
        }
        _broadcastTimer.Restart();

        try {
            auto& transform = SYS.GetComponent<CoreTransform>(_entity);
            serv::bytes args(std::vector<int>({ _id,
                static_cast<int>(transform.x),
                static_cast<int>(transform.y) }));
            auto instruction = serv::Instruction(eng::RType::I_ENEMY_MOVES, 0, args);
            eng::Engine::GetEngine()->GetServer().Broadcast(instruction.ToBytes() + serv::SEPARATOR);
        } catch (const std::exception& e) {
            std::cerr << "AIController::broadcastPosition(): " << e.what() << std::endl;
        }
    }

    void AIController::broadcastShoot()
    {
        if (not eng::Engine::GetEngine()->IsServer()) // only server can broadcast
            return;
        try {
            auto& transform = SYS.GetComponent<CoreTransform>(_entity);
            serv::bytes args(std::vector<int>({ _id,
                static_cast<int>(transform.x),
                static_cast<int>(transform.y) }));
            auto instruction = serv::Instruction(eng::RType::I_ENEMY_SHOOTS, 0, args);
            eng::Engine::GetEngine()->GetServer().Broadcast(instruction);

        } catch (const std::exception& e) {
            std::cerr << "AIController::broadcastShoot(): " << e.what() << std::endl;
        }
    }

    void AIController::instantiateRedLaser()
    {
        try {
            auto laser = SYS.GetResourceManager().LoadPrefab("red-laser");
            auto& transform = SYS.GetComponent<CoreTransform>(_entity);
            auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
            laserTransform.x = transform.x;
            laserTransform.y = transform.y;
        } catch (const std::exception& e) {
            std::cerr << "AIController::instantiateRedLaser(): " << e.what() << std::endl;
        }
    }

} // namespace rtype
