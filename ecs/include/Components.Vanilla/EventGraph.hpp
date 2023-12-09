/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** EventGraph.hpp
*/

#pragma once

#include "../Component.hpp"
#include "INode.hpp"
#include "metadata.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

serialize class EventGraph : public ecs::Component<EventGraph> {
public:
    GENERATE_METADATA(EventGraph)

    static void RegisterNode(const std::string& name, ecs::INode* defaultNode);
    static std::map<std::string, std::unique_ptr<ecs::INode>> AvailableNodes;

    EventGraph();

    void OnLoad();
    void Update(int entityID);

    std::vector<std::string> GetAvailableNodes() const;
    void AddNode(ecs::INode* node);

private:
    std::vector<ecs::INode*> Nodes; // TODO make this an actual graph
};
