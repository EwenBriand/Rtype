/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.cpp
*/

#include "DistantPlayer.hpp"
#include "ServerUdp.hpp"
#include <iostream>

serv::bytes DistantPlayer::HandleRequest(const serv::bytes& data)
{
    std::cout << "DistantPlayer::HandleRequest: " << data.toString() << std::endl;
    return serv::bytes("OK") + serv::SEPARATOR;
}

std::shared_ptr<serv::IClient> DistantPlayer::Clone()
{
    return std::make_shared<DistantPlayer>();
}