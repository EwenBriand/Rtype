/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** NodeStr.hpp
*/

#ifndef D4985EBC_FDC1_4897_9213_D50564BE7975
#define D4985EBC_FDC1_4897_9213_D50564BE7975
#include <iostream>
#include "INode.hpp"
#include "../Component.hpp"
#include "metadata.hpp"

needs(-lconfig++)
/**
 * @brief The node is simply a container for some text.
 *
 */
serialize class NodeStr : public virtual ecs::ANode {
    public:
        GENERATE_METADATA(NodeStr)

        /**
         * @brief Does Nothing. The node is simply a container for the text.
         *
         * @param entityID
         */
        void Execute(int entityID) override;

        /**
         * @brief Registers the node type in the EventGraph class.
         *
         */
        void OnLoad();

        /**
         * @brief Checks for user input and updates the text.
         *
         */
        void Update(int entityID);

    private:
        serialize std::string output_text = "placeholder text";
};


#endif /* D4985EBC_FDC1_4897_9213_D50564BE7975 */
