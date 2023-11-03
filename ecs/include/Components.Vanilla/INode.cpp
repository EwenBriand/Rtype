/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** INode.cpp
*/

#include <iostream>
#include "ECSImpl.hpp"
#include "INode.hpp"
#include "../Component.hpp"

namespace ecs {
    void ANode::OnAddComponent(int entityID)
    {
        std::cout << "on add component node" << std::endl;
        EventGraph *eventGraph;
        try {
            eventGraph = &(Sys.GetComponent<EventGraph>(entityID));
        } catch (std::exception &e) {
            Sys.AddComponent<EventGraph>(entityID);
            eventGraph = &(Sys.GetComponent<EventGraph>(entityID));
        }
        eventGraph->AddNode(this);
    }

    void ANode::Draw()
    {
    }
}
