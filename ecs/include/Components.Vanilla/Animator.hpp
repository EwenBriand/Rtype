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
        /**
         * @brief Exposes a few callbacks to be callables from buttons
         *
         */
        void editorRegisterUIButtonExposed();

        /**
         * @brief Saves the current entity to a file. If the entity hasn't been
         * given a name, it doesn't save and outputs a warning in the console.
         *
         */
        void saveCurrAnim();


        serialize std::string _currentAnimation = "";
        serialize std::vector<std::string> _registeredAnimations;

        std::map<std::string, Animation> _animations;

        std::string _animEditorAnimationName = "";
};
