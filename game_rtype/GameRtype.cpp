#include "GameRtype.hpp"
#include "DistantPlayer.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "LobbyCoroutine.hpp"
#include "LocalPlayerController.hpp"
#include "RTypeDistantServer.hpp"
#include "SceneManager.hpp"
#include "Ship.hpp"
#include <iostream>
#include <memory>

extern "C" {
std::shared_ptr<eng::IGame> create()
{
    std::cout << "Loading game_rtype" << std::endl;
    return std::make_shared<eng::RType>();
}
}

namespace eng {

    // =========================================================================================================== //
    // =============================================== PUBLIC ==================================================== //
    // =========================================================================================================== //

    void RType::Init(eng::Engine* e)
    {
        loadDependencies(e);
    }

    void RType::Cleanup(eng::Engine* e)
    {
    }

    bool RType::IsOnLine(eng::Engine* e)
    {
        return true;
    }

    void RType::WaitConnect(eng::Engine* e)
    {
        std::cout << "Waiting for connection" << std::endl;
        initServerConnection(e);
    }

    void RType::LoadFirstScene(eng::Engine* e)
    {
        if (not e->IsOptionSet(eng::Engine::Options::NO_GRAPHICS))
            e->GetSceneManager().SwitchScene("menu");
    }

    void RType::PreSceneInstantiationHook(eng::Engine* e, const std::string& sceneName)
    {
        if (sceneName == "lobby" and not e->IsOptionSet(eng::Engine::Options::SERVER_MODE)) {
            _stateMachine.SetState(std::make_shared<rtype::LobbyRoutineClient>(*e));
        }
    }

    // =========================================================================================================== //
    // ============================================== PRIVATE ==================================================== //
    // =========================================================================================================== //

    void RType::loadDependencies(Engine*)
    {
    }

    void RType::initServerConnection(Engine* e)
    {
        std::cout << "======================" << std::endl;
        if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE))
            std::cout << "Starting server mode" << std::endl;
        if (e->IsOptionSet(eng::Engine::Options::SERVER_IP))
            std::cout << "Server IP: " << e->GetOptionValue(eng::Engine::Options::SERVER_IP) << std::endl;
        if (e->IsOptionSet(eng::Engine::Options::SERVER_PORT))
            std::cout << "Server port: " << e->GetOptionValue(eng::Engine::Options::SERVER_PORT) << std::endl;
        if (!e->IsOptionSet(eng::Engine::Options::SERVER_MODE) && e->IsOptionSet(eng::Engine::Options::SERVER_IP) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
            std::cout << "Connecting to server" << std::endl;
            connectToServer(e);
            std::cout << "Connected to server" << std::endl;
        } else if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
            startServer(e);
        } else {
            std::cout << "Starting in single player mode" << std::endl;
        }
        std::cout << "======================" << std::endl;
    }

    void RType::startServer(Engine* e)
    {
        try {
            e->GetServer().SetHandleRequest<DistantPlayer>();
            e->GetServer().Start();
            _stateMachine = ecs::States(std::make_shared<rtype::LobbyRoutineServer>(*e));
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(84);
        }
    }

    void RType::connectToServer(Engine* e)
    {
        std::cout << "Connecting to server" << std::endl;
        std::string rawIP = e->GetOptionValue(eng::Engine::Options::SERVER_IP);
        std::cout << "IP: " << rawIP << std::endl;
        std::string rawPort = e->GetOptionValue(eng::Engine::Options::SERVER_PORT);
        std::cout << "Port: " << rawPort << std::endl;

        if (rawIP.empty() || rawPort.empty()) {
            std::cerr << "Invalid IP address, format is <ip>:<port>" << std::endl;
            exit(84);
        }
        if (rawPort.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Invalid port number" << std::endl;
            exit(84);
        }
        std::cout << "Connecting to " << rawIP << ":" << rawPort << std::endl;
        try {
            e->GetClient().SetServerAddress(rawIP, std::stoi(rawPort));
            std::cout << "Server address set" << std::endl;
            std::shared_ptr<rtype::RTypeDistantServer> serverHandle = std::make_shared<rtype::RTypeDistantServer>(e->GetClient());
            serverHandle->SetAsInstance();
            std::cout << "Server handle set" << std::endl;
            serverHandle->SetEngine(e);
            std::cout << "Engine set" << std::endl;
            e->GetClient().SetRequestHandler(serverHandle);
            std::cout << "Request handler set" << std::endl;
            e->GetClient().Start();
            std::cout << "Client started" << std::endl;
            _stateMachine = ecs::States(nullptr);
            std::cout << "State machine set" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(84);
        }
    }

    void RType::ModPipeline(Engine* e)
    {
        if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE)) {
            e->AddToPipeline([e, this]() {
                e->GetServer().CallHooks();
                _stateMachine.Iter();
            },
                -899, "RTYPE SERVER ROUTINE");
        } else if (e->IsOptionSet(eng::Engine::Options::SERVER_IP) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
            e->AddToPipeline([e, this]() {
                e->GetClient().CallHook();
                _stateMachine.Iter();
            },
                -899, "RTYPE CLIENT ROUTINE");
        }
    }

} // namespace eng
