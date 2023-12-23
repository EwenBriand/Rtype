/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Timer.hpp
*/

#pragma once

#include <chrono>

namespace eng {
    /**
     * @brief A simple timer that returns the elapsed time in seconds.
     *
     */
    class Timer {
    public:
        Timer();
        ~Timer() = default;

        /**
         * @brief Starts the timer.
         *
         */
        void Start();

        /**
         * @brief Pauses the timer
         *
         */
        void Stop();

        /**
         * @brief Resets the timer to 0.
         *
         */
        void Reset();

        /**
         * @brief Returns the elapsed time in seconds.
         *
         * @return double
         */
        double GetElapsedTime() const;

        /**
         * @brief Restarts the timer from zero seconds
         *
         */
        void Restart();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> _start;
        std::chrono::time_point<std::chrono::high_resolution_clock> _end;
        bool _isRunning;
    };
} // namespace eng
