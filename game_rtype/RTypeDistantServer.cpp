/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** RTypeDistantServer.cpp
*/

#include "RTypeDistantServer.hpp"
#include "AIController.hpp"
#include "CoreTransform.hpp"
#include "Enemy.hpp"
#include "GameRtype.hpp"
#include "LocalPlayerController.hpp"
#include "NetworkExceptions.hpp"
#include "PlayerFromServerController.hpp"
#include "Ship.hpp"

namespace rtype {
    const std::vector<std::string> RTypeDistantServer::PlayerPrefabs = {
        "red-ship",
        "infra",
        "micro-recon",
        "runner",
    };

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
        if (_pingThread.joinable())
            _pingThread.join();

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
        std::cout << "\rAssigned player id: " << _playerId << std::endl;
    }

    void RTypeDistantServer::handlePlayerSpawn(serv::Instruction& instruction)
    {
        int id = 0;
        if (instruction.data.size() != 3 * sizeof(int)) {
            throw std::runtime_error("Player spawn instruction has wrong data size.");
        }
        std::memcpy(&id, instruction.data.data(), sizeof(int));

        if (_players.find(id) != _players.end()) {
            return;
        }
        if (id == _playerId and _isAssignedLocalPlayer)
            return;
        try {
            auto entityID = _engine->GetECS().GetResourceManager().LoadPrefab("ship");
            Ship& ship = _engine->GetECS().GetComponent<Ship>(entityID, "Ship");

            auto& transform = _engine->GetECS().GetComponent<CoreTransform>(entityID);
            int x = 0;
            int y = 0;

            instruction.data.Deserialize(x, y);
            transform.x = x;
            transform.y = y;

            if (id == _playerId and not _isAssignedLocalPlayer) {
                _isAssignedLocalPlayer = true;
                auto lpc = std::make_shared<LocalPlayerController>();
                lpc->SetPlayerId(id);
                _entityID = entityID;
                lpc->SetEntity(_entityID);
                ship.Possess(_entityID, lpc);
                _observer = _engine->RegisterObserver();
                _observer->RegisterTarget([this]() { sendPlayerMoves(_entityID); }, transform.x, transform.y);
            } else {
                auto pfsc = std::make_shared<PlayerFromServerController>();
                pfsc->SetPlayerId(id);
                _players[id] = pfsc;
                ship.Possess(entityID, pfsc);
            }
            setPlayerAnimation(id, entityID);
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

        std::memcpy(&id, instruction.data.data(), sizeof(int));
        std::memcpy(&x, instruction.data.data() + sizeof(int), sizeof(int));
        std::memcpy(&y, instruction.data.data() + 2 * sizeof(int), sizeof(int));
        prefabName.resize(instruction.data.size() - 3 * sizeof(int));

        std::memcpy(&prefabName[0], instruction.data.data() + 3 * sizeof(int), instruction.data.size() - 3 * sizeof(int));

        try {
            auto eid = _engine->GetECS().GetResourceManager().LoadPrefab(prefabName);
            auto& transform = _engine->GetECS().GetComponent<CoreTransform>(eid);
            transform.x = x;
            transform.y = y;

            auto& ship = _engine->GetECS().GetComponent<Enemy>(eid, "Enemy");
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
            CONSOLE::err << "Failed to spawn laser: " << e.what() << std::endl;
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

    void RTypeDistantServer::handleEnemyDies(serv::Instruction& instruction)
    {
        if (instruction.data.size() != sizeof(int)) {
            throw serv::MalformedInstructionException("Enemy dies instruction malformed");
        }
        int id = 0;
        instruction.data.Deserialize(id);

        if (_enemies.find(id) == _enemies.end()) {
            return;
        }
        int entityID = _enemies[id];
        try {
            SYS.UnregisterEntity(entityID);
            _enemies.erase(id);
        } catch (const std::exception& e) {
            CONSOLE::err << "\r" << e.what() << std::endl;
        }
        std::cout << "\rEnemy " << id << " died." << std::endl;
        std::shared_ptr<eng::RType> game = std::dynamic_pointer_cast<eng::RType>(_engine->GetGame());
        game->GetSessionData().killCount++;
    }

    void RTypeDistantServer::setPlayerAnimation(int id, int entity)
    {
        if (id < 0 || id >= PlayerPrefabs.size()) {
            return; // not a player
        }
        try {
            auto& animator = SYS.GetComponent<Animator>(entity);
            animator.Play(PlayerPrefabs[id % PlayerPrefabs.size()]);
        } catch (const std::exception& e) {
            CONSOLE::err << "\r" << e.what() << std::endl;
        }
    }

    void RTypeDistantServer::Reset()
    {
        _isConnected = false;
        _startGame = false;
        _isAssignedLocalPlayer = false;
        _currSceneName = "";
        _playerId = 0;
        _entityID = -1;
        _isAssignedLocalPlayer = false;
        _players.clear();
        _enemies.clear();
        _engine->UnregisterObserver(_observer);

        auto rtype = std::dynamic_pointer_cast<eng::RType>(_engine->GetGame());
        rtype->GetSessionData().killCount = 0;

        _reset = true;
    }

    bool RTypeDistantServer::ShouldReset()
    {
        return _reset;
    }

    void RTypeDistantServer::ResetReset()
    {
        _reset = false;
    }

    void RTypeDistantServer::handleResetSignal(serv::Instruction& instruction)
    {
        if (RTypeDistantServer::GetInstance() != nullptr)
            RTypeDistantServer::GetInstance()->Reset();
    }

    void RTypeDistantServer::handleDisconnect(serv::Instruction& instruction)
    {
        Reset();
    }
} // namespace rtype
