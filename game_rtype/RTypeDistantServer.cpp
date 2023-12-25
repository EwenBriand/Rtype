/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.cpp
*/

#include "RTypeDistantServer.hpp"
#include "CoreTransform.hpp"
#include "LocalPlayerController.hpp"
#include "NetworkExceptions.hpp"
#include "Ship.hpp"

namespace rtype {
    RTypeDistantServer* RTypeDistantServer::Instance = nullptr;

    RTypeDistantServer::RTypeDistantServer(serv::ClientUDP& client)
        : AClientRequestHandler(client)
    {
    }

    RTypeDistantServer::~RTypeDistantServer()
    {
        _isConnected = false;
        if (_pingThread.joinable())
            _pingThread.join();
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

    RTypeDistantServer* RTypeDistantServer::GetInstance()
    {
        if (Instance == nullptr)
            throw std::runtime_error("RTypeDistantServer instance is null");
        return Instance;
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

    void RTypeDistantServer::SetEngine(eng::Engine* eng)
    {
        _engine = eng;
    }

    bool RTypeDistantServer::ShouldStartGame()
    {
        return _startGame;
    }

    bool RTypeDistantServer::SceneIsReady()
    {
        if (_currSceneName == "") {
            return false;
        }
        return _engine->GetSceneManager().IsSceneReady(_currSceneName);
    }

    void RTypeDistantServer::InstantiateScene()
    {
        if (_currSceneName == "")
            throw std::runtime_error("Cannot instantiate scene with no name.");
        _engine->GetSceneManager()
            .SwitchScene(_currSceneName);
    }

    // ========================================================================
    // REQUEST HANDLING METHODS
    // ========================================================================

    void RTypeDistantServer::handleConnectOk(serv::Instruction& instruction)
    {
        _isConnected = true;
        _pingThread = std::thread(&RTypeDistantServer::pingServerForAlive, this);
    }

    void RTypeDistantServer::handleServerFull(serv::Instruction& instruction)
    {
        std::cerr << "Server is full. Try looking for another server. Now shutting the client down." << std::endl;
        exit(0);
    }

    void RTypeDistantServer::handleLoadScene(serv::Instruction& instruction)
    {
        std::string sceneName = instruction.data.toString();
        _engine->GetSceneManager().LoadSceneAsync(sceneName);
        _currSceneName = sceneName;
        _startGame = false;
    }

    void RTypeDistantServer::handleStartGame(serv::Instruction& instruction)
    {
        _startGame = true;
    }

    void RTypeDistantServer::handleMessage(serv::Instruction& instruction)
    {
        std::cout << "\rMessage from server: " << instruction.data.toString() << std::endl;
    }

    void RTypeDistantServer::pingServerForAlive()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        while (_isConnected) {
            try {
                _client.Send(serv::Instruction(serv::I_AM_ALIVE, 0, serv::bytes()));
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void RTypeDistantServer::handleAssignPlayerID(serv::Instruction& instruction)
    {
        _playerId = instruction.data.ToInt();
    }

    void RTypeDistantServer::handlePlayerSpawn(serv::Instruction& instruction)
    {
        int id = 0;
        std::cout << "Spawning player" << std::endl;
        if (instruction.data.size() != 3 * sizeof(int)) {
            throw std::runtime_error("Player spawn instruction has wrong data size.");
        }
        std::memcpy(&id, instruction.data.data(), sizeof(int));

        try {
            int e = _engine->GetECS().GetResourceManager().LoadPrefab("ship");
            Ship& ship = _engine->GetECS().GetComponent<Ship>(e, "Ship");

            if (id == _playerId)
                ship.Possess(e, std::make_shared<LocalPlayerController>());
            else
                throw std::runtime_error("Possessing other players is not implemented yet."); // todo

            auto& transform = _engine->GetECS().GetComponent<CoreTransform>(e);
            std::memcpy(&transform.x, instruction.data.data() + sizeof(int), sizeof(int));
            std::memcpy(&transform.y, instruction.data.data() + 2 * sizeof(int), sizeof(int));
        } catch (std::exception& e) {
            std::cerr << "\r" << e.what() << std::endl;
        }
    }
} // namespace rtype
