/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** EventGraph.hpp
*/

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "../Component.hpp"
#include "metadata.hpp"
#include "INode.hpp"

needs(-lconfig++)
serialize class EventGraph : public ecs::Component<EventGraph> {
    public:
        GENERATE_METADATA(EventGraph)

        static void RegisterNode(const std::string &name, ecs::INode *defaultNode);
        static std::map<std::string, std::unique_ptr<ecs::INode>> AvailableNodes;

        EventGraph();

        void OnLoad();
        void Update(int entityID);

        std::vector<std::string> GetAvailableNodes() const;
        void AddNode(ecs::INode *node);
    private:
        std::vector<ecs::INode *> Nodes; // TODO make this an actual graph
};
