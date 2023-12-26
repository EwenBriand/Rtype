/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Timer.cpp
*/

#include "Timer.hpp"

namespace eng {
    Timer::Timer()
    {
        Reset();
    }

    void Timer::Start()
    {
        _start = std::chrono::high_resolution_clock::now();
        _isRunning = true;
    }

    void Timer::Stop()
    {
        _end = std::chrono::high_resolution_clock::now();
        _isRunning = false;
    }

    void Timer::Reset()
    {
        _start = std::chrono::high_resolution_clock::now();
        _end = std::chrono::high_resolution_clock::now();
        _isRunning = false;
    }

    double Timer::GetElapsedTime() const
    {
        if (_isRunning) {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double>(now - _start).count();
        } else {
            return std::chrono::duration<double>(_end - _start).count();
        }
    }

    void Timer::Restart()
    {
        Reset();
        Start();
    }
} // namespace eng
