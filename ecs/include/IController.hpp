/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** IController.hpp
*/

#pragma once
#include <map>
#include <string>
#include <vector>

namespace ecs {
    /**
     * @brief This interface is used to define a controller,
     * meaning a class that will be used to control an entity.
     * It will expose the directives from the player / ai / network
     * so that they are accessible from the entity.
     *
     */
    class IController {
    public:
        /**
         * @brief Returns a vector containing all the directives received
         * during the current frame.
         * This vector gets cleared at the end of the frame, and must be filled before
         * the system's --Update-- routine is called.
         * This can be achieved by modifying the engine's pipeline.
         *
         * @example
         * a map containing the following directives:
         * {
         *     "up",
         *     "shoot"
         * }
         *
         * note that the directives should contain actions to perform by the controlled object,
         * not the state of the keys. This should be taken into account in
         * the controller's internal logic.
         *
         * @return std::vector<std::string>&
         */
        virtual std::vector<std::string>& GetDirectives() = 0;

        /**
         * @brief Updates the state of the directives, either from the
         * local player, the network, or the ai.
         *
         */
        virtual void PollDirectives() = 0;

        /**
         * @brief Should insert the controller in the engine's pipeline,
         * before the --Update-- routine.
         *
         */
        virtual void UpdatePipeline() = 0;

        /**
         * @brief Sets the internal reference to the entity's controller.
         *
         */
        virtual void SetEntity(int entityID) = 0;
    };

    class AController : public IController {
    public:
        AController();
        virtual std::vector<std::string>& GetDirectives() = 0;
        virtual void PollDirectives() = 0;

        virtual void UpdatePipeline() override;
        virtual void SetEntity(int entityID) override;
        virtual int GetEntity() const;

    protected:
        int _entity;
    };
} // namespace ecs
