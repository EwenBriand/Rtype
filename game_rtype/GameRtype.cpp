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
        if (e->IsClient()) {
            connectToServer(e);
        } else if (e->IsServer()) {
            startServer(e);
        } else {
            std::cout << "Starting in single player mode" << std::endl;
        }
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
        std::string rawIP = e->GetOptionValue(eng::Engine::Options::SERVER_IP);
        std::string rawPort = e->GetOptionValue(eng::Engine::Options::SERVER_PORT);

        if (rawIP.empty() || rawPort.empty()) {
            std::cerr << "Invalid IP address, format is <ip>:<port>" << std::endl;
            exit(84);
        }
        if (rawPort.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Invalid port number" << std::endl;
            exit(84);
        }
        try {
            e->GetClient().SetServerAddress(rawIP, std::stoi(rawPort));
            std::shared_ptr<rtype::RTypeDistantServer> serverHandle = std::make_shared<rtype::RTypeDistantServer>(e->GetClient());
            serverHandle->SetAsInstance();
            serverHandle->SetEngine(e);
            e->GetClient().SetRequestHandler(serverHandle);
            e->GetClient().Start();
            _stateMachine = ecs::States(nullptr);
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
