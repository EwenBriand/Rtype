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
#include "Observer.hpp"

namespace rtype {
    std::vector<std::string>& AIController::GetDirectives()
    {
        try {
            const auto& transform = SYS.GetComponent<CoreTransform>(_entity);
            if (transform.x < -100) {
                eng::Engine::GetEngine()->UnregisterObserver(_observer);
                SYS.UnregisterEntity(_entity);
            }
        } catch (const std::exception& e) {
            std::cerr << "AIController::GetDirectives(): " << e.what() << std::endl;
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
    }

    bool AIController::testLeft()
    {
        return true;
    }

    bool AIController::testShoot()
    {
        if (_shootTimer.GetElapsedTime() > _shootInterval) {
            _shootTimer.Restart();
            return true;
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

} // namespace rtype
