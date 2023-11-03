/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Engine.cpp
*/

#include "Components.Vanilla/WowGraphics.hpp"
#include <libconfig.h++>
#include <filesystem>
#include <variant>
#include <memory>
#include <algorithm>
#include "Component.hpp"
#include "Engine.hpp"
#include "ECSImpl.hpp"
#include "GraphicalRayLib/GraphicalRayLib.hpp"
#include "SceneManager.hpp"

namespace eng
{
    std::ofstream Engine::m_logFile = std::ofstream(".engine/log.txt", std::ios::out | std::ios::trunc);

    Engine *Engine::GetEngine()
    {
        static std::unique_ptr<Engine> engine;

        if (!engine) {
            engine = std::make_unique<Engine>();
        }
        return engine.get();
    }

    Engine::Engine() :
        m_graphicalModule(Sys.GetResourceManager().LoadGraphicalModule())
    {
        m_preUpdatePipeline = std::make_shared<std::vector<Action>>();
        m_postUpdatePipeline = std::make_shared<std::vector<Action>>();
        m_unsortedPipeline = std::make_shared<std::vector<std::tuple<int, std::tuple<std::string, Action>>>>();
    }

    Engine::~Engine()
    {
        m_logFile.close();
    }

    void Engine::setupPipeline()
    {
        pushPipeline([&]() {
            Sys.GetInputManager().PollEvents();
        }, -1000);
        m_graphicalModule->ModPipeline();
    }

    void Engine::sortPipeline()
    {
        std::string pipelineAsStr = "";
        bool preUpdate = true;

        std::sort(m_unsortedPipeline->begin(), m_unsortedPipeline->end(), [](const std::tuple<int, std::tuple<std::string, Action>> &a, const std::tuple<int, std::tuple<std::string, Action>> &b) {
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
            Console::info << "More information on the pipeline in .pipinfo.txt" << std::endl;
        } catch (std::exception &e) {
            Console::warn << "Could not save pipeline information" << std::endl;
        }
        m_pipelineChanged = false;
    }

    void Engine::NotifyPipelineChange()
    {
        m_pipelineChanged = true;
    }

    void Engine::AddToPipeline(Action action, int priority, const std::string &name)
    {
        m_unsortedPipeline->push_back(std::make_tuple(priority, std::make_tuple(name, action)));
        m_pipelineChanged = true;
    }

    void Engine::ClearPipeline()
    {
        m_unsortedPipeline->clear();
        m_preUpdatePipeline->clear();
        m_postUpdatePipeline->clear();
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
            discoverConfig(m_options["--config-dir"]);
        } catch (EngineException &e) {
            Console::err << "\nWhile discovering config: \n" << e.what() << std::endl;
            Stop();
        }
        Sys.SetGraphicalModule(m_graphicalModule);
        Sys.GetInputManager().SetupDefaults();
        if (IsOptionSet("--editor")) {
            Sys.AddComponent<CLI>(Sys.GetSystemHolder());
            Sys.AddComponent<EditorMouseManager>(Sys.GetSystemHolder());
            Sys.AddComponent<EntityExplorer>(Sys.GetSystemHolder());
            ecs::SceneManager::Get().InitEditorMode();
        }

        Sys.LoadVanilla();
        Sys.GetResourceManager().CheckHotReload();

        for (auto e : Sys.GetEntities()) {
            Sys.ForEachComponent(e, [&](ecs::ECSImpl::AnyCpt &cpt) {
                std::visit([&](auto &&arg) {
                    arg.Start();
                }, cpt);
            });
        }
        m_graphicalModule->Start();
        setupPipeline();
        sortPipeline();

        if (IsOptionSet("--wow-graphics")) {
            wow::WowGraphicsEngine::Enable();
        }
        Sys.CallStart();
        Sys.Run(
            [&]() {
                for (auto action : *m_preUpdatePipeline) {
                    if (Sys.FrameIsSkipped()) {
                        return;
                    }
                    action();
                }
            },
            [&]() {
                if (m_pipelineChanged) {
                    Console::info << "Updating pipeline" << std::endl;
                    sortPipeline();
                }
                for (auto action : *m_postUpdatePipeline) {
                    if (Sys.FrameIsSkipped())
                        break;
                    action();
                }
            }
        );
        m_graphicalModule->Stop();
    }

    bool Engine::IsOptionSet(const std::string &optionName)
    {
        return m_options.find(optionName) != m_options.end();
    }

    void Engine::Stop()
    {
    }

    void Engine::discoverConfig(const std::string &configDir)
    {
        std::vector<std::string> configFiles;
        std::string configExt = ".cfg";
        std::string configPath = std::filesystem::current_path().string() + "/" + configDir;

        Console::info << "Discovering config files in: " << configPath << std::endl;
        if (!std::filesystem::exists(configPath))
            throw EngineException("Config directory not found", __FILE__, __FUNCTION__, __LINE__);
        for (auto &p : std::filesystem::directory_iterator(configPath)) {
            if (p.path().extension() == configExt)
                configFiles.push_back(p.path().string());
        }
        if (configFiles.size() == 0)
            throw EngineException("No config file found", __FILE__, __FUNCTION__, __LINE__);
        for (auto &file : configFiles) {
            Console::info << "Loading config file: " << file << std::endl;
            loadConfig(file);
        }
        std::string savePath = GetConfigValue("scenesSavePath");
        if (!std::filesystem::exists(savePath))
            std::filesystem::create_directory(savePath);
        Console::info << "Data will be saved at " << savePath << std::endl;
        Sys.SetSavePath(savePath);

    }

    void Engine::loadConfig(const std::string &path)
    {
        libconfig::Config cfg;
        cfg.readFile(path.c_str());

        libconfig::Setting &root = cfg.getRoot();
        for (auto &setting : root) {
            std::string name = setting.getName();
            std::string value = setting.c_str();
            std::cout << "\t -- " << name << " = " << value << std::endl;
            m_config[name] = value;
        }
    }

    void Engine::ParseOptions(int argc, char **argv)
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

    std::string Engine::GetOptionValue(const std::string &option)
    {
        if (m_options.find(option) == m_options.end())
            throw EngineException("Option not found : " + option, __FILE__, __FUNCTION__, __LINE__);
        return m_options[option];
    }

    std::string Engine::GetConfigValue(const std::string &config)
    {
        try {
            return m_config.at(config);
        } catch (std::out_of_range &e) {
            throw EngineException("Config not found : " + config, __FILE__, __FUNCTION__, __LINE__);
        }
    }

    void Engine::Log(const std::string &msg)
    {
        m_logFile << msg << std::endl;
    }
} // namespace eng

