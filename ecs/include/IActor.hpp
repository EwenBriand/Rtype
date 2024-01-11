/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** IActor.hpp
*/

#pragma once

#include "IController.hpp"
#include <memory>

namespace ecs {
    /**
     * @brief This class is used to define an actor, meaning an entity
     * that can be controlled by a controller (player, ai, network, etc.)
     *
     */
    class IActor {
    public:
        virtual void Possess(int entity, std::shared_ptr<IController>) = 0;
    };

    class AActor : public IActor {
    public:
        void Possess(int entity, std::shared_ptr<IController> controller) override;

    protected:
        std::shared_ptr<IController> _controller = nullptr;
    };
}
