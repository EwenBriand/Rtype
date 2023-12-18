/*
** EPITECH PROJECT, 2023
** repo
** File description:
** SceneManager.cpp
*/

#include "SceneManager.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"

namespace eng {
    void SceneManager::LoadSceneAsync(const std::string& sceneName)
    {
        _scenes[sceneName] = std::make_shared<SceneBuffer>(sceneName, m_engine);
    }

    void SceneManager::InitEditorMode()
    {
        Entity sysHolder = SYS.GetSystemHolder();
        CLI& cli = SYS.GetComponent<CLI>(sysHolder);

        cli.RegisterCustomCommand(
            "scm", [this](CLI& c, const std::vector<std::string>& args) {
                if (args.size() != 2) {
                    CONSOLE::err << "Usage: scmLoadScene <sceneName>" << std::endl;
                    return;
                }
                LoadSceneAsync(args[0]);
            },
            "Loads a scene asynchronously");

        cli.RegisterCustomCommand(
            "scmsw", [this](CLI& c, const std::vector<std::string>& args) {
                if (args.size() != 2) {
                    CONSOLE::err << "Usage: scmSwitchScene <sceneName>" << std::endl;
                    return;
                }
                SwitchScene(args[0]);
            },
            "Switches to the given scene");

        cli.RegisterCustomCommand(
            "scmdel", [this](CLI& c, const std::vector<std::string>& args) {
                if (args.size() != 2) {
                    CONSOLE::err << "Usage: scmDelScene <sceneName>" << std::endl;
                    return;
                }
                UnloadScene(args[0]);
            },
            "Unloads the given scene");
    }

    bool SceneManager::IsSceneReady(const std::string& sceneName)
    {
        try {
            auto& s = *(_scenes.at(sceneName));
            return s.IsReady();
        } catch (std::exception& e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return false;
        }
    }

    void SceneManager::SwitchScene(const std::string& sceneName)
    {
        std::cout << "loading scene" << std::endl;
        if (_scenes.find(sceneName) == _scenes.end()) {
            LoadSceneAsync(sceneName);
        }
        while (!IsSceneReady(sceneName)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        try {
            m_engine.GetECS().ReplaceScene(_scenes.at(sceneName)->GetComponents());
            auto game = m_engine.GetGame();
            if (game)
                game->PreSceneInstantiationHook(&m_engine, sceneName);
        } catch (std::exception& e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return;
        }
    }

    void SceneManager::UnloadScene(const std::string& sceneName)
    {
        if (_scenes.find(sceneName) == _scenes.end()) {
            CONSOLE::err << "Error: scene [" << sceneName << "] not found" << std::endl;
            return;
        }
        _scenes.erase(sceneName);
    }

    SceneBuffer::SceneBuffer(const std::string& sceneName, Engine& engine)
    {
        _isReady = false;
        _future = std::async(std::launch::async, [this, sceneName, &engine]() {
            load(sceneName, engine);
            _isReady = true;
        });
    }

    bool SceneBuffer::IsReady() const
    {
        return _isReady;
    }

    void SceneBuffer::load(const std::string& sceneName, Engine& engine)
    {
        try {

            std::string path = engine.GetConfigValue("scenesSavePath")
                + "/" + sceneName;
            _components = engine.GetECS().PrepareScene(path);
            int entityNumber = _components[0].size();
            CONSOLE::info << "Scene [" << sceneName << "] OK\nLoaded " << ecs::yellow << entityNumber << ecs::green << " entities" << std::endl;

        } catch (std::exception& e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return;
        }
    }

    ecs::ECSImpl::AllCpt& SceneBuffer::GetComponents()
    {
        return _components;
    }

    SceneManager::SceneManager(Engine& engine)
        : m_engine(engine)
    {
    }

} // namespace eng
