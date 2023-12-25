/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** PlayerFromServerController.hpp
*/

#pragma once
#include "IController.hpp"

namespace rtype {
    class PlayerFromServerController : public ecs::AController {
    public:
        // ====================================================================
        // IController methods
        // ====================================================================
        std::vector<std::string>& GetDirectives() override;
        void PollDirectives() override;

        // ====================================================================
        // public methods
        // ====================================================================

        inline void SetPlayerId(int id) { _playerID = id; }

        // ====================================================================
        // getters/setters
        // ====================================================================

    private:
        // ====================================================================
        // private methods
        // ====================================================================

        // ====================================================================
        // properties
        // ====================================================================
        std::vector<std::string> _directives;

        int _playerID;
    };
} // namespace rtype
