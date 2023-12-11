/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** NodePrintStr.hpp
*/

#ifndef DCA450FB_2CDA_414A_A39C_DC37B5B6E867
#define DCA450FB_2CDA_414A_A39C_DC37B5B6E867
#include "../Component.hpp"
#include "INode.hpp"
#include "metadata.hpp"
#include <iostream>

serialize class NodePrintStr : public virtual ecs::ANode {
public:
    GENERATE_METADATA(NodePrintStr)

    /**
     * @brief Prints the text in the console.
     *
     * @param entityID
     */
    void Execute(int entityID) override;

    /**
     * @brief Registers the node type in the EventGraph class.
     *
     */
    void OnLoad();

private:
    serialize std::string input_text = "";
};

#endif /* DCA450FB_2CDA_414A_A39C_DC37B5B6E867 */
