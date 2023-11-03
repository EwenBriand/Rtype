/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Animator.hpp
*/

#pragma once
#include <map>
#include "../Component.hpp"
#include "metadata.hpp"

class Animation {

};

needs(-lconfig++)
serialize class Animator : public ecs::Component<Animator> {
    public:
        GENERATE_METADATA(Animator);

        void OnLoad();
    private:
        serialize std::string _currentAnimation = "";
        serialize std::vector<std::string> _registeredAnimations;

        std::map<std::string, Animation> _animations;
};
