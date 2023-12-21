/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** States.hpp
*/

#pragma once
#include <memory>

namespace ecs {

    /**
     * @brief An interface for a state of a state machine.
     *
     */
    class IState {
    public:
        virtual ~IState() = default;

        /**
         * @brief the enter function executes the state's logic.
         *
         */
        virtual void Enter() = 0;

        /**
         * @brief the exit function returns the next state of the state machine,
         * or nullptr if the state does not change.
         *
         * @return std::shared_ptr<IState>
         */
        virtual std::shared_ptr<IState> Exit() = 0;
    };

    /**
     * @brief The States class is a state machine, which can be used to
     * manage the state of a game.
     *
     */
    class States {
    public:
        States();
        States(std::shared_ptr<IState> initialState);
        ~States();

        /**
         * @brief Sets the current state of the state machine.
         *
         * @param state
         */
        void SetState(std::shared_ptr<IState> state);

        /**
         * @brief Runs the current's state logic.
         *
         */
        void Iter();

    private:
        std::shared_ptr<IState> _currentState;
    };
}
