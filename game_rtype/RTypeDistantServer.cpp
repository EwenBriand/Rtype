/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.cpp
*/

#include "RTypeDistantServer.hpp"
#include "AIController.hpp"
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
        serv::Instruction instruction;
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
                _client.Send(serv::Instruction(serv::I_AM_ALIVE, 0, serv::bytes()).ToBytes() + serv::SEPARATOR);
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
        if (instruction.data.size() != 3 * sizeof(int)) {
            throw std::runtime_error("Player spawn instruction has wrong data size.");
        }
        std::memcpy(&id, instruction.data.data(), sizeof(int));

        try {
            auto entityID = _engine->GetECS().GetResourceManager().LoadPrefab("ship");
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
                lpc->SetPlayerId(id);
                _entityID = entityID;
                lpc->SetEntity(_entityID);
                ship.Possess(_entityID, lpc);
                _engine->RegisterObserver()->RegisterTarget([this]() { sendPlayerMoves(_entityID); }, transform.x, transform.y);
            } else {
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

    void RTypeDistantServer::handleEnemySpawn(serv::Instruction& instruction)
    {
        if (instruction.data.size() < 3 * sizeof(int)) {
            throw std::runtime_error("Enemy spawn instruction has wrong data size.");
        }
        int id = 0;
        int x = 0;
        int y = 0;
        std::string prefabName = "";

        instruction.data.Deserialize(id, x, y);
        serv::bytes prefabNameBytes(instruction.data.data() + 3 * sizeof(int), instruction.data.size() - 3 * sizeof(int));
        prefabName.reserve(prefabNameBytes.size());
        for (auto& byte : prefabNameBytes) {
            prefabName += byte;
        }

        try {
            auto eid = _engine->GetECS().GetResourceManager().LoadPrefab(prefabName);
            auto& transform = _engine->GetECS().GetComponent<CoreTransform>(eid);
            transform.x = x;
            transform.y = y;

            auto& ship = _engine->GetECS().GetComponent<Ship>(eid, "Ship");
            auto pfsc = std::make_shared<rtype::AIController>();
            pfsc->SetID(id);
            ship.Possess(eid, pfsc);
            _enemies[id] = eid;
        } catch (std::exception& e) {
            std::cerr << "\r" << e.what() << std::endl;
        }
    }

    void RTypeDistantServer::handleEnemyMoves(serv::Instruction& instruction)
    {
        int id = 0;
        int x = 0;
        int y = 0;
        if (instruction.data.size() < 3 * sizeof(int)) {
            throw std::runtime_error("Enemy moves instruction has wrong data size.");
        }
        instruction.data.Deserialize(id, x, y);

        if (_enemies.find(id) == _enemies.end()) {
            return;
        }
        int entityID = _enemies[id];
        try {
            auto& tr = _engine->GetECS().GetComponent<CoreTransform>(entityID);
            tr.x = x;
            tr.y = y;
        } catch (std::exception& e) {
            std::cerr << "\r" << e.what() << std::endl;
        }
    }

    void RTypeDistantServer::handlePlayerShoots(serv::Instruction& instruction)
    {
        if (instruction.data.size() != 3 * sizeof(int)) {
            throw serv::MalformedInstructionException("Player shoots instruction malformed");
        }
        int id = 0;
        int x = 0;
        int y = 0;
        instruction.data.Deserialize(id, x, y);

        try {
            int laser = SYS.GetResourceManager().LoadPrefab("Laser");
            auto& transform = SYS.GetComponent<CoreTransform>(laser);
            transform.x = x;
            transform.y = y;
        } catch (const std::exception& e) {
            CONSOLE::err << "\rFailed to send shoot instruction to server." << std::endl;
        }
    }

    void RTypeDistantServer::handleEnemyShoots(serv::Instruction& instruction)
    {
        try {
            int id, x, y;
            instruction.data.Deserialize(id, x, y);

            int laser = SYS.GetResourceManager().LoadPrefab("red-laser");
            auto& transform = SYS.GetComponent<CoreTransform>(laser);
            transform.x = x;
            transform.y = y;
        } catch (const std::exception& e) {
            CONSOLE::warn << "\r" << e.what() << std::endl;
        }
    }
} // namespace rtype
