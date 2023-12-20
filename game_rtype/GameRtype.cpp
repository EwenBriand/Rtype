#include "GameRtype.hpp"
#include "DistantPlayer.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "LocalPlayerController.hpp"
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
        e->GetSceneManager().SwitchScene("menu");
    }

    void RType::PreSceneInstantiationHook(eng::Engine* e, const std::string& sceneName)
    {
        if (sceneName == "menu")
            return;

        // update this to account for networking, other players, etc
        int player = SYS.GetResourceManager().LoadPrefab("ship");
        Ship* ship = GetUComponent(player, Ship);
        ship->Possess(player, std::make_shared<LocalPlayerController>());
    }

    // =========================================================================================================== //
    // ============================================== PRIVATE ==================================================== //
    // =========================================================================================================== //

    void RType::loadDependencies(Engine*)
    {
    }

    void RType::initServerConnection(Engine* e)
    {
        if (!e->IsOptionSet(eng::Engine::Options::SERVER_MODE) && e->IsOptionSet(eng::Engine::Options::SERVER_IP) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
            std::cout << "Connecting to server at " << e->GetOptionValue(eng::Engine::Options::SERVER_IP) << std::endl;
            connectToServer(e);
        } else if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
            std::cout << "Starting server at port " << e->GetOptionValue(eng::Engine::Options::SERVER_MODE) << std::endl;
            startServer(e);
        } else {
            std::cout << "Starting in single player mode" << std::endl;
        }
    }

    void RType::startServer(Engine* e)
    {
        if (e->GetOptionValue(eng::Engine::Options::SERVER_PORT).find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Invalid port number" << std::endl;
            exit(84);
        }

        int port = std::stoi(e->GetOptionValue(eng::Engine::Options::SERVER_PORT));
        try {
            e->GetServer().SetPort(port);
            e->GetServer().Start();
            e->GetServer().RegisterHandshake<DistantPlayer>("DistantPlayer");
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
            e->GetClient().Connect(rawIP, std::stoi(rawPort), "Handshake: DistantPlayer\r\n");
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(84);
        }
    }

    void RType::ModPipeline(Engine* e)
    {
        if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE)) {
            e->pushPipeline([e]() {
                e->GetServer().Poll();
            },
                -899);
        } else if (e->IsOptionSet(eng::Engine::Options::SERVER_IP) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
            e->pushPipeline([e]() {
                e->GetClient().Receive();
            },
                -899);
        }
    }
} // namespace eng