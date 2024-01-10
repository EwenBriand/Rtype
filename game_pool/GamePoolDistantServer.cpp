/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** GamePoolDistantServer.cpp
*/

#include "GamePoolDistantServer.hpp"

namespace pool {
    void GamePoolDistantServer::HandleRequest(const serv::bytes& data)
    {
        serv::Instruction instruction;
        if (_reset)
            return;
        try {
            instruction = serv::Instruction(data);
            if (_requestHandlers.find(instruction.opcode) != _requestHandlers.end()) {
                auto handler = _requestHandlers.at(instruction.opcode);
                if (handler)
                    (this->*handler)(instruction);
            } else {
                std::cerr << "\rUnknown instruction received from server: " << instruction.opcode << std::endl;
            }
        } catch (const serv::MalformedInstructionException& e) {
            std::cerr << "\rMore informations: \n";
            std::cerr << "\r\traw size: " << data.size() << std::endl;
            std::cerr << "\r\tOpcode: " << instruction.opcode << std::endl;
            std::cerr << "\r\tExpects answer: " << instruction.expectsAnswer << std::endl;
            std::cerr << "\r\tData size: " << instruction.data.size() << std::endl;
            std::cerr << "\r\tData to string: " << data.toString() << std::endl;
            std::cerr << "\r\tData: ";
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}