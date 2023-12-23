/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.cpp
*/

#include "RTypeDistantServer.hpp"
#include "NetworkExceptions.hpp"

namespace rtype {
    RTypeDistantServer* RTypeDistantServer::Instance = nullptr;

    RTypeDistantServer::RTypeDistantServer(serv::ClientUDP& client)
        : AClientRequestHandler(client)
    {
    }

    void RTypeDistantServer::HandleRequest(const serv::bytes& data)
    {
        try {
            serv::Instruction instruction(data);
            if (_requestHandlers.find(instruction.opcode) != _requestHandlers.end())
                (this->*_requestHandlers[instruction.opcode])(instruction);
            else {
                std::cerr << "Unknown instruction received from server: " << instruction.opcode << std::endl;
            }
        } catch (const serv::MalformedInstructionException& e) {
            std::cerr << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void RTypeDistantServer::SetAsInstance()
    {
        Instance = this;
    }

    RTypeDistantServer& RTypeDistantServer::GetInstance()
    {
        if (Instance == nullptr)
            throw std::runtime_error("RTypeDistantServer instance is null");
        return *Instance;
    }

    void RTypeDistantServer::TryConnect()
    {
        if (_isConnected)
            return;
        try {
            _client.Send(serv::Instruction(serv::I_CONNECT, 0, serv::bytes()));
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    bool RTypeDistantServer::IsConnected() const
    {
        return _isConnected;
    }

    // ========================================================================
    // REQUEST HANDLING METHODS
    // ========================================================================

    void RTypeDistantServer::handleConnectOk(serv::Instruction& instruction)
    {
        std::cout << "Connected to server" << std::endl;
        _isConnected = true;
    }

    void RTypeDistantServer::handleServerFull(serv::Instruction& instruction)
    {
        std::cerr << "Server is full. Try looking for another server. Now shutting the client down." << std::endl;
        exit(0);
    }
} // namespace rtype
