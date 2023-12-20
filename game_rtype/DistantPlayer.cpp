/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** DistantPlayer.cpp
*/

#include "DistantPlayer.hpp"

serv::ServerImpl::Message DistantPlayer::HandleRequest(const std::string& data)
{
    std::cout << "[LOG] from server [" << data << "]" << std::endl;
    return serv::ServerImpl::Message("OK");
}