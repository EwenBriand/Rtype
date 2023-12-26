/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** PlayerFromServer.cpp
*/

#include "PlayerFromServerController.hpp"

namespace rtype {

    std::vector<std::string>& PlayerFromServerController::GetDirectives()
    {
        return _directives;
    }

    void PlayerFromServerController::PollDirectives()
    {
        _directives.clear();
    }
}