/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** EventGraph.cpp
*/

#include <string>
#include <vector>
#include "../ECSImpl.hpp"
#include "EventGraph.hpp"
#include "CLI.hpp"

decltype(EventGraph::AvailableNodes) EventGraph::AvailableNodes;

EventGraph::EventGraph()
{

}

void EventGraph::OnLoad()
{
    Entity system = SYS.GetSystemHolder();
    try {
        CLI &cli = SYS.GetComponent<CLI>(system);
        cli.RegisterCustomCommand("evtgraph-addnode", [&](CLI &c, std::vector<std::string> args) {
            CONSOLE::warn << "evtgraph-addnode: " << args[0] << " not yet implemented" << std::endl;
        });
        cli.RegisterCustomCommand("evtgraph-listnodes", [&](CLI &c, std::vector<std::string> args) {
            std::vector<std::string> nodes = GetAvailableNodes();
            for (auto &node : nodes)
                CONSOLE::info << node << std::endl;
        });
    } catch (std::exception &e) {}
}

void EventGraph::Update(int entityID)
{
    for (auto &node : Nodes)
        node->Execute(entityID);
}

void EventGraph::RegisterNode(const std::string &name, ecs::INode *defaultNode)
{
    AvailableNodes.emplace(name, defaultNode);
}

std::vector<std::string> EventGraph::GetAvailableNodes() const
{
    std::vector<std::string> ret;
    for (auto &node : AvailableNodes)
        ret.push_back(node.first);
    return ret;
}

void EventGraph::AddNode(ecs::INode *node)
{
    Nodes.push_back(node);
}
