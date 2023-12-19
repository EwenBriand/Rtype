/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.hpp
*/

#pragma once
#include "Server.hpp"

class DistantPlayer : public serv::ServerImpl::IClient {
public:
    serv::ServerImpl::Message HandleRequest(const std::string& data) override;
};