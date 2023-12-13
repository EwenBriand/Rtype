/*
** EPITECH PROJECT, 2023
** repo
** File description:
** SceneManager.cpp
*/

#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "SceneManager.hpp"
#include "Components.Vanilla/CLI.hpp"

namespace ecs
{
    std::unique_ptr<SceneManager> SceneManager::_instance = nullptr;

    SceneManager &SceneManager::Get()
    {
        if (_instance == nullptr)
            _instance = std::make_unique<SceneManager>();
        return *_instance;
    }

    void SceneManager::LoadSceneAsync(const std::string &sceneName)
    {
        _scenes.emplace(sceneName, sceneName);
    }

    void SceneManager::InitEditorMode()
    {
        Entity sysHolder = SYS.GetSystemHolder();
        CLI &cli = SYS.GetComponent<CLI>(sysHolder);

        cli.RegisterCustomCommand("scm", [this](CLI &c, const std::vector<std::string> &args) {
            if (args.size() != 2) {
                CONSOLE::err << "Usage: scmLoadScene <sceneName>" << std::endl;
                return;
            }
            LoadSceneAsync(args[0]);
        });

        cli.RegisterCustomCommand("scmsw", [this](CLI &c, const std::vector<std::string> &args) {
            if (args.size() != 2) {
                CONSOLE::err << "Usage: scmSwitchScene <sceneName>" << std::endl;
                return;
            }
            SwitchScene(args[0]);
        });

        cli.RegisterCustomCommand("scmdel", [this](CLI &c, const std::vector<std::string> &args) {
            if (args.size() != 2) {
                CONSOLE::err << "Usage: scmDelScene <sceneName>" << std::endl;
                return;
            }
            UnloadScene(args[0]);
        });
    }

    bool SceneManager::IsSceneReady(const std::string &sceneName)
    {
        if (_scenes.find(sceneName) == _scenes.end()) {
            LoadSceneAsync(sceneName);
        }
        try {
            auto &s = _scenes.at(sceneName);
            return s.IsReady();
        } catch (std::exception &e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return false;
        }
    }

    void SceneManager::SwitchScene(const std::string &sceneName)
    {
        if (_scenes.find(sceneName) == _scenes.end()) {
            LoadSceneAsync(sceneName);
        }
        while (!IsSceneReady(sceneName)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        try {
            SYS.ReplaceScene(_scenes.at(sceneName).GetComponents());
        } catch (std::exception &e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return;
        }
    }


    void SceneManager::UnloadScene(const std::string &sceneName)
    {
        if (_scenes.find(sceneName) == _scenes.end()) {
            CONSOLE::err << "Error: scene [" << sceneName << "] not found" << std::endl;
            return;
        }
        _scenes.erase(sceneName);
    }

    SceneBuffer::SceneBuffer(const std::string &sceneName)
    {
        _isReady = false;
        _future = std::async(std::launch::async, [this, sceneName]() {
            load(sceneName);
            _isReady = true;
        });
    }

    bool SceneBuffer::IsReady() const
    {
        return _isReady;
    }

    void SceneBuffer::load(const std::string &sceneName)
    {
        try {

            std::string path = eng::Engine::GetEngine()->GetConfigValue("scenesSavePath")
                + "/" + sceneName;
            _components = SYS.PrepareScene(path);
            int entityNumber = _components[0].size();
            CONSOLE::info << "Scene [" << sceneName << "] OK\nLoaded " << ecs::yellow << entityNumber << ecs::green << " entities" << std::endl;

        } catch (std::exception &e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return;
        }
    }

    ECSImpl::AllCpt &SceneBuffer::GetComponents()
    {
        return _components;
    }
} // namespace ecs
