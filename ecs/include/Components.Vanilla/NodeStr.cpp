/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** NodeStr.cpp
*/

#include "EventGraph.hpp"
#include "NodeStr.hpp"

void NodeStr::Execute(__attribute__((unused)) int entityID)
{
}

void NodeStr::OnLoad()
{
    EventGraph::RegisterNode("NodeStr", new NodeStr());
}

void NodeStr::Update(__attribute__((unused)) int entityID)
{
    // TODO check for user input and update the text
}
