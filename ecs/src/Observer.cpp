/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Observer.cpp
*/

#include "Observer.hpp"

namespace eng {
    void Observer::Poll()
    {
        if (_timeout > 0) {
            if (_timer.GetElapsedTime() > _timeout) {
                _timer.Reset();
                if (_callback)
                    _callback();
            }
        }
        bool hasChanged = false;
        for (auto& target : _targets) {
            if (target.HasChanged()) {
                target.Update();
                hasChanged = true;
            }
        }
        if (hasChanged && _callback) {
            _callback();
        }
    }

    void Observer::SetTimeout(float timeout)
    {
        _timeout = timeout;
    }
}