/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Engine.cpp
*/

// #include "Components.Vanilla/WowGraphics.hpp"
#include "Engine.hpp"
#include "Component.hpp"
#include "ECSImpl.hpp"
#include "GraphicalRayLib/GraphicalRayLib.hpp"
#include "SceneManager.hpp"
#include "UIManager.hpp"
#include <algorithm>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <variant>

namespace eng {
    std::ofstream Engine::m_logFile = std::ofstream(".engine/log.txt", std::ios::out | std::ios::trunc);
    const std::string Engine::Options::EDITOR = "--editor";
    const std::string Engine::Options::GAME = "game";
    const std::string Engine::Options::CONFIG_DIR = "--config-dir";
    const std::string Engine::Options::VERBOSE = "--verbose";
    const std::string Engine::Options::NO_GRAPHICS = "--no-graphics";
    const std::string Engine::Options::SERVER_IP = "--server-ip";
    const std::string Engine::Options::SERVER_PORT = "--server-port";
    const std::string Engine::Options::SERVER_MODE = "--server-mode";

    ecs::ECSImpl& Engine::GetECS()
    {
        return m_ecs;
    }

    Engine* Engine::GetEngine()
    {
        static std::unique_ptr<Engine> engine;

        if (!engine) {
            engine = std::make_unique<Engine>();
        }
        return engine.get();
    }

    Engine::Engine()
        : m_graphicalModule(nullptr)
        , m_ecs(SYS)
    {
        m_preUpdatePipeline = std::make_shared<std::vector<Action>>();
        m_postUpdatePipeline = std::make_shared<std::vector<Action>>();
        m_unsortedPipeline = std::make_shared<std::vector<std::tuple<int, std::tuple<std::string, Action>>>>();
        m_sceneManager = std::make_shared<eng::SceneManager>(*this);
    }

    Engine::~Engine()
    {
        m_logFile.close();
    }

    void Engine::setupPipeline()
    {
        pushPipeline([&]() {
            SYS.GetInputManager().PollEvents();
            for (auto& observer : m_observers) {
                observer->Poll();
            }
        },
            -1000);
        ui::UIManager::Get().ModPipeline();
        m_graphicalModule->ModPipeline();
        if (m_game)
            m_game->ModPipeline(this);
    }

    void Engine::sortPipeline()
    {
        std::string pipelineAsStr = "";
        bool preUpdate = true;

        std::sort(m_unsortedPipeline->begin(), m_unsortedPipeline->end(), [](const std::tuple<int, std::tuple<std::string, Action>>& a, const std::tuple<int, std::tuple<std::string, Action>>& b) {
            return std::get<0>(a) < std::get<0>(b);
        });
        m_preUpdatePipeline->clear();
        m_postUpdatePipeline->clear();

        for (auto [prio, action] : *m_unsortedPipeline) {
            if (prio < 0)
                m_preUpdatePipeline->push_back(std::get<1>(action));
            else
                m_postUpdatePipeline->push_back(std::get<1>(action));
            if (preUpdate && prio >= 0) {
                preUpdate = false;
                pipelineAsStr += "--- Update() ---\n";
            }
            pipelineAsStr += std::get<0>(action) + "\n";
        }
        try {
            std::ofstream pipinfo(".pipinfo.txt", std::ios::out | std::ios::trunc);
            pipinfo << pipelineAsStr;
            pipinfo.close();
            CONSOLE::info << "\rMore information about the pipeline in .pipinfo.txt" << std::endl;
        } catch (std::exception& e) {
            CONSOLE::warn << "\rCould not save pipeline information" << std::endl;
        }
        m_pipelineChanged = false;
    }

    void Engine::SetupEditor()
    {
        if (IsOptionSet(eng::Engine::Options::EDITOR)) {
            SYS.AddComponent<CLI>(SYS.GetSystemHolder());
            SYS.AddComponent<EditorMouseManager>(SYS.GetSystemHolder());
            SYS.AddComponent<EntityExplorer>(SYS.GetSystemHolder());
            m_sceneManager->InitEditorMode();
        }
    }

    void Engine::NotifyPipelineChange()
    {
        m_pipelineChanged = true;
    }

    void Engine::AddToPipeline(Action action, int priority, const std::string& name)
    {
        m_unsortedPipeline->push_back(std::make_tuple(priority, std::make_tuple(name, action)));
        m_pipelineChanged = true;
    }

    void Engine::ClearPipeline()
    {
        m_unsortedPipeline->clear();
        m_preUpdatePipeline->clear();
        m_postUpdatePipeline->clear();
        m_observers.clear();
        m_pipelineChanged = true;
        setupPipeline();
    }

    std::shared_ptr<graph::IGraphicalModule> Engine::GetGraphicalModule() const
    {
        return m_graphicalModule;
    }

    void Engine::Run()
    {
        try {
            discoverConfig(m_options[eng::Engine::Options::CONFIG_DIR]);
        } catch (EngineException& e) {
            CONSOLE::err << "\nWhile discovering config: \n"
                         << e.what() << std::endl;
            Stop();
        }
        try {
            m_graphicalModule = SYS.GetResourceManager().LoadGraphicalModule("");
        } catch (ConfigException& e) {
            CONSOLE::err << "\nWhile loading graphical module: \n"
                         << e.what() << std::endl;
            return;
        }
        try {
            loadNetworkModules();
        } catch (EngineException& e) {
            CONSOLE::err << "\nWhile loading server module: \n"
                         << e.what() << std::endl;
            return;
        }
        SYS.SetGraphicalModule(m_graphicalModule);
        SetupEditor();
        try {
            m_game = SYS.GetResourceManager().LoadGame(GetConfigValue(eng::Engine::Options::GAME));
            CONSOLE::info << "Project loaded" << std::endl;
        } catch (ConfigException& e) {
            CONSOLE::warn << "No project loaded" << std::endl;
            m_game = nullptr; // no game loaded
        } catch (EngineException& e) {
            CONSOLE::err << "\nWhile loading game: \n"
                         << e.what() << std::endl;
        }

        SYS.LoadVanilla();
        std::cout << "Vanilla loaded" << std::endl;
        if (m_game != nullptr) {
            m_game->Init(this);
            std::cout << "Game initialized" << std::endl;
        }

        SYS.GetResourceManager().CheckHotReload();
        std::cout << "Starting game" << std::endl;
        for (auto e : SYS.GetEntities()) {
            SYS.ForEachComponent(e, [&](ecs::ECSImpl::AnyCpt& cpt) {
                std::visit([&](auto&& arg) {
                    arg.Start();
                },
                    cpt);
            });
        }
        std::cout << "Game started" << std::endl;

        m_graphicalModule->Start();
        std::cout << "Graphical module started" << std::endl;
        if (m_game && m_game->IsOnLine(this)) {
            std::cout << "Waiting for connection" << std::endl;
            m_game->WaitConnect(this);
        }
        std::cout << "Starting main loop" << std::endl;

        setupPipeline();
        std::cout << "Pipeline setup" << std::endl;
        sortPipeline();
        std::cout << "Pipeline sorted" << std::endl;

        SYS.CallStart();
        if (m_game)
            m_game->LoadFirstScene(this);
        SYS.Run(
            [&]() {
                for (auto action : *m_preUpdatePipeline) {
                    if (SYS.FrameIsSkipped()) {
                        return;
                    }
                    action();
                }
            },
            [&]() {
                if (m_pipelineChanged) {
                    CONSOLE::info << "\rUpdating pipeline" << std::endl;
                    sortPipeline();
                }
                for (auto action : *m_postUpdatePipeline) {
                    if (SYS.FrameIsSkipped())
                        break;
                    action();
                }
            });
        m_graphicalModule->Stop();
        if (m_game)
            m_game->Cleanup(this);
        m_game.reset();
    }

    SceneManager& Engine::GetSceneManager()
    {
        if (!m_sceneManager)
            throw std::runtime_error("Scene manager is null.");
        return *m_sceneManager;
    }

    bool Engine::IsOptionSet(const std::string& optionName)
    {
        return m_options.find(optionName) != m_options.end();
    }

    void Engine::Stop()
    {
        exit(0);
    }

    void Engine::configUpdateRelativePath()
    {
        std::string configDir = GetOptionValue(Options::CONFIG_DIR);

        for (auto& [key, value] : m_config) {
            if ((value.size() > 2) && (value[0] == '.') && (value[1] == '/')
                || (value.size() > 3) && (value[0] == '.') && (value[1] == '.') && (value[2] == '/')) {
                value = std::filesystem::canonical(configDir + value.substr(2)).string();
            }
        }
        #ifdef _WIN32
        for (auto& [key, value] : m_config) {
            std::replace(value.begin(), value.end(), '/', '\\');
        }
        #endif
    }

    void Engine::discoverConfig(const std::string& configDir)
    {
        std::vector<std::string> configFiles;
        std::string configExt = ".cfg";
        std::string configPath = configDir;

        CONSOLE::info << "Discovering config files in: " << configPath << std::endl;
        if (!std::filesystem::exists(configPath))
            throw EngineException("Config directory not found : " + configPath, __FILE__, __FUNCTION__, __LINE__);
        for (auto& p : std::filesystem::directory_iterator(configPath)) {
            if (p.path().extension() == configExt)
                configFiles.push_back(p.path().string());
        }
        if (configFiles.size() == 0)
            throw EngineException("No config file found", __FILE__, __FUNCTION__, __LINE__);
        for (auto& file : configFiles) {
            CONSOLE::info << "Loading config file: " << file << std::endl;
            loadConfig(file);
        }
        configUpdateRelativePath();
        std::string savePath = GetConfigValue("scenesSavePath");
        for (auto [key, value] : m_config) {
            CONSOLE::info << "\t" << key << " = " << GetConfigValue(key) << std::endl;
        }
        if (!std::filesystem::exists(savePath))
            std::filesystem::create_directory(savePath);
        CONSOLE::info << "Data will be saved at " << savePath << std::endl;
        SYS.SetSavePath(savePath);
    }

    void Engine::loadConfig(const std::string& path)
    {
        std::ifstream config_file(path);
        if (!config_file.is_open()) {
            throw std::runtime_error("Could not open config file: " + path);
        }

        std::string line;
        while (std::getline(config_file, line)) {
            if (line.empty() || line.front() == '#' || line.front() == '\n')
                continue;

            std::istringstream iss(line);
            std::string key, value;
            if (!std::getline(iss, key, '=') || !std::getline(iss, value)) {
                throw std::runtime_error("Invalid config grammar: " + line);
            }

            key.erase(0, key.find_first_not_of(' '));
            key.erase(key.find_last_not_of(' ') + 1);

            value.erase(0, value.find_first_not_of(' '));
            value.erase(value.find_last_not_of(' ') + 1);

            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            if (key.empty() || value.empty()) {
                throw std::runtime_error("Invalid config grammar: " + line);
            }
            m_config[key] = value;
        }

        config_file.close();
    }

    void Engine::ParseOptions(int argc, char** argv)
    {
        std::string tmp;
        std::string optionName;
        std::string optionValue;

        for (int i = 0; i < argc; ++i) {
            tmp = std::string(argv[i]);
            auto equalIndex = tmp.find('=');
            if (equalIndex == std::string::npos) {
                optionName = tmp;
                optionValue = "true";
            } else {
                optionName = tmp.substr(0, equalIndex);
                optionValue = tmp.substr(equalIndex + 1);
            }
            m_options[optionName] = optionValue;
        }
    }

    std::string Engine::GetOptionValue(const std::string& option)
    {
        if (m_options.find(option) == m_options.end())
            throw EngineException("Option not found : " + option, __FILE__, __FUNCTION__, __LINE__);
        return m_options[option];
    }

    std::string Engine::GetConfigValue(const std::string& config)
    {
        try {
            return m_config.at(config);
        } catch (std::out_of_range& e) {
            throw ConfigException("Config not found : " + config);
        }
    }

    void Engine::Log(const std::string& msg)
    {
        m_logFile << msg << std::endl;
    }

    std::shared_ptr<IGame> Engine::GetGame() const
    {
        return m_game;
    }

    void Engine::loadNetworkModules()
    {
        std::cout << "Loading network modules" << std::endl;
        if (IsOptionSet(eng::Engine::Options::SERVER_MODE) && IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
            std::cout << "Starting in server mode" << std::endl;
            std::string port = GetOptionValue(eng::Engine::Options::SERVER_PORT);
            std::cout << "Port: " << port << std::endl;
            if (port.find_first_not_of("0123456789") != std::string::npos) {
                throw EngineException("Invalid port number", __FILE__, __FUNCTION__, __LINE__);
            }
            std::cout << "Starting server" << std::endl;
            m_server = std::make_shared<serv::ServerUDP>(std::stoi(port));
            CONSOLE::warn << "Server listening on port " << port << std::endl;
        } else if (IsOptionSet(eng::Engine::Options::SERVER_IP)) {
            std::cout << "Starting in client mode" << std::endl;
            m_client = std::make_shared<serv::ClientUDP>();
            std::cout << "Connecting to server" << std::endl;
        } else {
            CONSOLE::warn << "Starting in single player mode" << std::endl;
        }
    }

    bool Engine::PlayMode() const
    {
        return m_playMode;
    }

    void Engine::SetPlayMode(bool playMode)
    {
        m_playMode = playMode;
    }

    bool Engine::IsClient() const
    {
        return m_client != nullptr;
    }

    bool Engine::IsServer() const
    {
        return m_server != nullptr;
    }

    void Engine::UnregisterObserver(std::shared_ptr<Observer> observer)
    {
        if (not observer)
            return;
        for (int i = 0; i < m_observers.size(); ++i) {
            if (m_observers[i] and m_observers[i].get() == observer.get()) {
                m_observers.erase(m_observers.begin() + i);
                return;
            }
        }
    }

    // serv::ServerUDP& Engine::GetServer() const
    // {
    //     if (!m_server)
    //         throw EngineException("Server not initialized", __FILE__, __FUNCTION__, __LINE__);
    //     return *m_server;
    // }

    // serv::ClientUDP& Engine::GetClient() const
    // {
    //     if (!m_client)
    //         throw EngineException("Client not initialized", __FILE__, __FUNCTION__, __LINE__);
    //     return *m_client;
    // }

    // ecs::ECSImpl& Engine::GetECS() const
    // {
    //     return SYS;
    // }
} // namespace eng
