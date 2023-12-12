/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** IGame.hpp
*/

#pragma once

namespace eng {

    /**
     * @brief Interface for the game class, which shall be loaded dynamically by the engine.
     * The classes inheriting from this interface will be given an occasion to initialize their
     * specific functionalities, as well as do some cleanup when the game is unloaded.
     *
     */
    class IGame {
    public:
        virtual ~IGame() = default;
        /**
         * @brief Called when the game is loaded by the engine.
         *
         */
        virtual void Init() = 0;

        /**
         * @brief Called when the game is unloaded by the engine.
         *
         */
        virtual void Cleanup() = 0;

        /**
         * @brief Returns true if the game is online, false otherwise.
         * If it returns true, the engine will call the WaitForPlayers method before launching
         * the game.
         *
         */
        virtual bool IsOnLine() = 0;

        /**
         * @brief Called when the game is online, before the game is launched.
         *
         */
        virtual void WaitConnect() = 0;
    };
}