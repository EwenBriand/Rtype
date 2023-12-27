/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.cpp
*/

#include "RTypeDistantServer.hpp"
#include "CoreTransform.hpp"
#include "GameRtype.hpp"
#include "LocalPlayerController.hpp"
#include "NetworkExceptions.hpp"
#include "PlayerFromServerController.hpp"
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
            if (_requestHandlers.find(instruction.opcode) != _requestHandlers.end()) {
                auto handler = _requestHandlers.at(instruction.opcode);
                if (handler)
                    (this->*handler)(instruction);
            } else {
                std::cerr << "\rUnknown instruction received from server: " << instruction.opcode << std::endl;
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
        std::cout << "\rAssigned player id: " << _playerId << " in entity " << _entityID << std::endl;
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
            auto entityID = _engine->GetECS().GetResourceManager().LoadPrefab("ship");
            std::cout << "entity id is " << entityID << std::endl;
            Ship& ship = _engine->GetECS().GetComponent<Ship>(entityID, "Ship");

            auto& transform = _engine->GetECS().GetComponent<CoreTransform>(entityID);
            int x = 0;
            int y = 0;
            std::memcpy(&x, instruction.data.data() + sizeof(int), sizeof(int));
            std::memcpy(&y, instruction.data.data() + 2 * sizeof(int), sizeof(int));
            transform.x = x;
            transform.y = y;

            if (id == _playerId) {
                auto lpc = std::make_shared<LocalPlayerController>();
                _entityID = entityID;
                std::cout << "now controlling entity " << _entityID << std::endl;
                lpc->SetEntity(_entityID);
                ship.Possess(_entityID, lpc);
                _engine->RegisterObserver()->RegisterTarget([this]() { sendPlayerMoves(_entityID); }, transform.x, transform.y);
            } else {
                std::cout << "Spawning player from server" << std::endl;
                auto pfsc = std::make_shared<PlayerFromServerController>();
                pfsc->SetPlayerId(id);
                _players[id] = pfsc;
                ship.Possess(entityID, pfsc);
            }
        } catch (std::exception& e) {
            std::cerr << "\r" << e.what() << std::endl;
        }
    }

    void RTypeDistantServer::sendPlayerMoves(int id)
    {
        auto& transform = _engine->GetECS().GetComponent<CoreTransform>(id);
        int x = transform.x;
        int y = transform.y;
        serv::bytes data(std::vector<int> { _playerId, x, y });
        _client.Send(serv::Instruction(eng::RType::I_PLAYER_MOVES, 0, data));
    }

    void RTypeDistantServer::handlePlayerMoves(serv::Instruction& instruction)
    {
        if (instruction.data.size() < 3 * sizeof(int)) {
            std::cout << "\rinstruction size: " << instruction.data.size() << std::endl;
            throw serv::MalformedInstructionException("Player moves instruction malformed");
        }
        int id = 0;
        int x = 0;
        int y = 0;
        std::memcpy(&id, instruction.data.data(), sizeof(int));
        std::memcpy(&x, instruction.data.data() + sizeof(int), sizeof(int));
        std::memcpy(&y, instruction.data.data() + 2 * sizeof(int), sizeof(int));

        if (_players.find(id) == _players.end()) {
            throw serv::MalformedInstructionException("Unknown player id: " + std::to_string(id));
        }
        int entityID = _players[id]->GetEntity();
        try {
            auto& transform = _engine->GetECS().GetComponent<CoreTransform>(entityID);
            transform.x = x;
            transform.y = y;
        } catch (std::exception& e) {
            std::cerr << "\r" << e.what() << std::endl;
        }
    }
} // namespace rtype
