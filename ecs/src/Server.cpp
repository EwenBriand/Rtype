/*
** EPITECH PROJECT, 2023
** ecs
** File description:
** Server.cpp
*/

#include "../include/Server.hpp"
#include "Engine.hpp"

namespace serv {
    void Server::Update(int entityID)
    {
        ServerImpl::Get()->Poll();
    }

    void Server::OnLoad()
    {

        ServerImpl::Get()->SetPort(8080);
    }

    void Server::Start()
    {
        ServerImpl::Get()->Start();
    }

    void Server::Load(const std::string& path)
    {
    }

    void Server::Save(const std::string& path)
    {
    }

    void Server::OnAddComponent(int entityID)
    {
    }

    std::map<std::string, metadata_t> Server::GetMetadata()
    {
        std::map<std::string, metadata_t> metadata;
        return metadata;
    }

    std::string Server::GetClassName() const
    {
        return "Server";
    }
}
