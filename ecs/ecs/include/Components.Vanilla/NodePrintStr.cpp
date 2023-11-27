/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** NodePrintStr.cpp
*/

#include "EventGraph.hpp"
#include "NodePrintStr.hpp"

void NodePrintStr::Execute(int entityID)
{
    if (input_text == "")
        return;
    std::cout << input_text << std::endl;
}

void NodePrintStr::OnLoad()
{
    EventGraph::RegisterNode("NodeStr", new NodePrintStr());
}
