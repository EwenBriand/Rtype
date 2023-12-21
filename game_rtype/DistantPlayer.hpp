/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.hpp
*/

#pragma once
#include "ServerUdp.hpp"

class DistantPlayer : public serv::IClient {
public:
    serv::bytes HandleRequest(const serv::bytes& data) override;
    std::shared_ptr<serv::IClient> Clone() override;
};