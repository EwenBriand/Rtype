/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ResourceManager.cpp
*/

#include "ResourceManager.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GraphicalRayLib/GraphicalRayLib.hpp"
#include "IGraphicalModule.hpp"
#include "LibUtils.hpp"
#include "NoGraphics.hpp"
#include "Timer.hpp"
#include "metadataGenerator.hpp"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <type_traits>
#include <unistd.h>
#include <yaml-cpp/yaml.h>

#ifdef _WIN32
#include <processthreadsapi.h>
#endif

namespace ecs {
    ResourceManager::~ResourceManager()
    {
        if (std::get<0>(_graphicalModule))
            lib::LibUtils::closeLibHandle(std::get<0>(_graphicalModule));
        // if (_gameHandle) {
        //     lib::LibUtils::closeLibHandle(_gameHandle);
        // }
        for (auto& plugin : _plugins) {
            if (plugin.second.handle)
                lib::LibUtils::closeLibHandle(plugin.second.handle);
        }
    }

    std::shared_ptr<AUserComponent> ResourceManager::LoadUserComponent(
        const std::string& resourceID)
    {
        if (_handles.find(resourceID) != _handles.end()) {
            AUserComponent* (*create)() = reinterpret_cast<AUserComponent* (*)()>(lib::LibUtils::getSymHandle(_handles[resourceID], "create_" + resourceID));
            _instances.emplace_back(create());
            return _instances.back();
        }
        #ifdef _WIN32
        std::string libName = m_userComponentsPath + "lib" + resourceID + ".dll";
        #else
        std::string libName = m_userComponentsPath + "lib" + resourceID + ".so";
        #endif
        std::cout << "Loading " << libName << std::endl;
        void* handle = lib::LibUtils::getLibHandle(libName.c_str());
        _handles[resourceID] = handle;
        AUserComponent* (*create)() = reinterpret_cast<AUserComponent* (*)()>(lib::LibUtils::getSymHandle(handle, "create_" + resourceID));
        _handles[resourceID] = handle;
        _instances.emplace_back(create());
        _instances.back()->OnLoad();
        return _instances.back();
    }

    bool ResourceManager::CompileUserScript(const std::string& path)
    {
        std::string tmpCopyDirectory = ".tmpCompileSrc";
        std::string rootDir = "./";
        std::string userScriptDir = eng::Engine::GetEngine()->GetConfigValue("userScriptsPath");
        try {
            rootDir = eng::Engine::GetEngine()->GetConfigValue("tmpBuildDir");
        } catch (std::exception& e) {
            CONSOLE::warn << "Error: " << e.what() << std::endl;
            CONSOLE::warn << "Could not find tmpBuildDir in config, using default value" << std::endl;
        }
        #ifdef _WIN32
        std::string rawPath = path.substr(0, path.find_last_of('.'));
        std::string copyPath = tmpCopyDirectory + "\\" + rawPath.substr(rawPath.find_last_of('\\') + 1);
        // std::string command = "mkdir " + copyPath + " && copy " + rawPath + ".cpp " +  copyPath + " && copy " + rawPath + ".hpp " + copyPath;
        std::string command = "rmdir /s /q " + copyPath + " && mkdir " + copyPath + " && copy " + rawPath + ".cpp " +  copyPath + " && copy " + rawPath + ".hpp " + copyPath;
        std::cout << "Executing command : " << command << std::endl;
        system(command.c_str());
        #else
        std::string rawPath = path.substr(0, path.find_last_of('.'));
        std::string rawFolder = path.substr(0, path.find_last_of('/'));
        std::string copyPath = rootDir + "/" + tmpCopyDirectory + "/" + rawPath.substr(rawPath.find_last_of('/') + 1);
        std::string luaLibs = eng::Engine::GetEngine()->GetConfigValue("luaHeaders");
        std::string luabridge = eng::Engine::GetEngine()->GetConfigValue("luaBridge");
        std::string command = "mkdir -p " + copyPath + " && cp " + rawPath + ".cpp " + rawPath + ".hpp " + copyPath;
        system(command.c_str());
        #endif
        try {
            #ifndef _WIN32 
            auto metagen = meta::MetadataGenerator();
            metagen.generateMetadata(copyPath, "./metabuild", rootDir, { userScriptDir, MakePath({ userScriptDir, ".." }), luaLibs, luabridge }, userScriptDir);
            metagen.buildCMake();
            #endif
        } catch (std::exception& e) {
            CONSOLE::err << "Build failed: " << path << std::endl;
            std::ofstream checksumPath(rawPath + ".checksum", std::ios::trunc | std::ios::out);
            checksumPath << "build failed, retry";
            checksumPath.close();
            return false;
        }
        return true;
    }

    bool ResourceManager::FileHasChanged(const std::string& path)
    {
        std::string rawPath = path.substr(0, path.find_last_of('.'));
        std::string checksumPath = rawPath + ".checksum";
        std::string checksum = GetChecksum(rawPath + ".cpp") + GetChecksum(rawPath + ".hpp");
        std::ifstream file(checksumPath);
        std::ofstream newFile;
        std::string tmp;
        bool changed = false;

        if (!file.is_open())
            changed = true;
        else {
            std::getline(file, tmp);
            if (tmp != checksum)
                changed = true;
        }
        file.close();
        newFile.open(checksumPath, std::ios::trunc | std::ios::out);
        newFile << checksum;
        newFile.close();
        return changed;
    }

    void ResourceManager::CheckHotReload()
    {
        std::string line;
        std::string userScriptDir = eng::Engine::GetEngine()->GetConfigValue("userScriptsPath");
        m_changesNbr = 0;
        try {
            for (auto& entry : std::filesystem::directory_iterator(userScriptDir)) {
                if (entry.path().extension() == ".cpp") {
                    CONSOLE::info << "Checking " << entry.path() << std::endl;
                    ManageUpdate(entry.path().string());
                }
            }
            if (m_changesNbr == 0)
                CONSOLE::info << "No changes detected in user scripts" << std::endl;
        } catch (std::runtime_error& e) {
            CONSOLE::err << "Compilation error during hot reload : " << e.what() << std::endl;
            CONSOLE::err << "Aborting" << std::endl;
            return;
        }
        if (m_changesNbr > 0)
            SYS.ReloadEntities();
    }

    void ResourceManager::ManageUpdate(const std::string& path)
    {
        if (FileHasChanged(path)) {
            ++m_changesNbr;
            CONSOLE::info << "Detected changes in source file " << path << std::endl;
            if (!CompileUserScript(path))
                throw std::runtime_error("Compilation failed");
        }
    }

    std::string ResourceManager::GetChecksum(const std::string& path)
    {
        std::ifstream file(path);
        std::string line;
        int checksum = 0;

        if (!file.is_open())
            return "";
        while (std::getline(file, line)) {
            for (char& c : line) {
                checksum *= 31;
                checksum += c;
            }
        }
        return std::to_string(checksum);
    }

    void ResourceManager::Clear()
    {
        for (auto& instance : _instances) {
            if (instance.use_count() > 1) {
                throw eng::EngineException("User component still in use : " + instance->GetClassName(), __FILE__, __FUNCTION__, __LINE__);
            }
        }
        _instances.clear();
        for (auto& handle : _handles) {
            lib::LibUtils::closeLibHandle(handle.second);
        }
        _handles.clear();
    }

    std::vector<std::string> ResourceManager::GetActionList() const
    {
        std::vector<std::string> v(_availableActions.size());
        int i = 0;

        for (auto [key, value] : _availableActions) {
            v[i++] = key;
        }
        return v;
    }

    void ResourceManager::RegisterPublicAction(const std::string& name, Action action)
    {
        if (_availableActions.find(name) == _availableActions.end())
            throw std::runtime_error("Action already registered");

        _availableActions[name] = action;
    }

    std::shared_ptr<graph::IGraphicalModule> ResourceManager::LoadGraphicalModule(
        const std::string& path)
    {
        if (std::get<1>(_graphicalModule) != nullptr) {
            throw eng::EngineException("Graphical module already loaded", __FILE__, __FUNCTION__, __LINE__);
        }
        if (!eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::NO_GRAPHICS)) {
            auto tmp = std::shared_ptr<graph::IGraphicalModule>(new raylib::GraphicalRayLib());
            _graphicalModule = std::make_tuple(nullptr, tmp);
            return std::get<1>(_graphicalModule);
        } else {
            auto tmp = std::shared_ptr<graph::IGraphicalModule>(new eng::NoGraphics());
            _graphicalModule = std::make_tuple(nullptr, tmp);
            return std::get<1>(_graphicalModule);
        }
    }

    std::string ResourceManager::LoadFileText(const std::string& path)
    {
        std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
        std::ifstream file(assetRoot + path);
        std::string line;
        std::string text;

        if (!file.is_open())
            throw eng::EngineException("File not found : " + path, __FILE__, __FUNCTION__, __LINE__);
        while (std::getline(file, line)) {
            text += line + "\n";
        }
        return text;
    }

    std::string ResourceManager::MakePath(const std::vector<std::string>& pathElements, bool throwIfNotExist)
    {
        std::filesystem::path p;
        for (const auto& element : pathElements) {
            p /= element;
        }
        p = std::filesystem::canonical(p);
        if (throwIfNotExist && !std::filesystem::exists(p)) {
            throw eng::EngineException("Path does not exist : " + p.string(), __FILE__, __FUNCTION__, __LINE__);
        }
        return p.string();
    }

    std::shared_ptr<eng::IGame> ResourceManager::LoadGame(const std::string& path)
    {
        if (_gameHandle) {
            throw eng::EngineException("Game already loaded", __FILE__, __FUNCTION__, __LINE__);
        }

        void* handle = lib::LibUtils::getLibHandle(path.c_str());
        std::shared_ptr<eng::IGame> (*create)() = reinterpret_cast<std::shared_ptr<eng::IGame> (*)()>(lib::LibUtils::getSymHandle(handle, "create"));
        _game = create();
        if (!_game) {
            throw eng::EngineException("Game creation failed", __FILE__, __FUNCTION__, __LINE__);
        }
        _gameHandle = handle;
        CONSOLE::info << "Game successfully loaded" << std::endl;
        return _game;
    }

    int ResourceManager::LoadPrefab(const std::string& name)
    {
        std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
        std::string path = MakePath({ assetRoot, "prefabs", name }, true);

        try {
            int e = SYS.LoadEntity(path);
            SYS.ForEachComponent(e, [&](auto& cpt) {
                std::visit([&](auto&& arg) {
                    arg.Start();
                },
                    cpt);
            });
            return e;
        } catch (std::exception& e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return -1;
        }
    }

    void ResourceManager::SavePrefab(const std::string& name, int entity)
    {
        std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
        std::string path = assetRoot + "/prefabs/" + name;

        try {
            if (not std::filesystem::exists(path)) {
                std::filesystem::create_directories(path);
            }
            CONSOLE::info << "Saving prefab " << name << " to " << path << std::endl;
            SYS.SaveEntity(entity, path);
        } catch (std::exception& e) {
            CONSOLE::err << "Error: " << e.what() << std::endl;
            return;
        }
    }

    void ResourceManager::HotLoadLibrary(const std::string& path)
    {
        std::string srcCpp = eng::Engine::GetEngine()->GetConfigValue("userScriptsPath");
        srcCpp += "/" + path + ".cpp";
        std::string srcHpp = eng::Engine::GetEngine()->GetConfigValue("userScriptsPath");
        srcHpp += "/" + path + ".hpp";

        ManageUpdate(srcCpp);
        ManageUpdate(srcHpp);
        #ifdef _WIN32
        std::string libName = m_userComponentsPath + "lib" + path + ".dll";
        #else
        std::string libName = m_userComponentsPath + "lib" + path + ".so";
        #endif
        void* handle = lib::LibUtils::getLibHandle(libName.c_str());
        std::cout << "done" << std::endl;
        _handles[path] = handle;
    }

    void ResourceManager::LoadPlugins()
    {
        try {
            std::string pluginPath = eng::Engine::GetEngine()->GetConfigValue("pluginPath");
            discoverPlugins(pluginPath);
        } catch (std::exception& e) {
            CONSOLE::warn << "Error: " << e.what() << std::endl;
            CONSOLE::warn << "Not loading any plugins" << std::endl;
        }
    }

    void ResourceManager::discoverPlugins(const std::string& path)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                std::string pluginName = entry.path().stem().string();
                if (pluginName.substr(0, 3) == "lib") {
                    pluginName = pluginName.substr(3);
                }
                std::string yamlFilePath = path + "/" + pluginName + "/" + pluginName + ".yaml";

                if (std::filesystem::exists(yamlFilePath)) {
                    try {
                        PluginInfo plugin = ParsePlugin(yamlFilePath, entry.path().string());
                        _plugins[pluginName] = plugin;
                    } catch (std::exception& e) {
                        CONSOLE::warn << e.what() << std::endl;
                        CONSOLE::warn << "Skipping plugin " << pluginName << std::endl;
                    }
                }
            }
        }
    }

    ResourceManager::PluginInfo ResourceManager::ParsePlugin(const std::string& path, const std::string& binaryPath)
    {
        PluginInfo plugin;
        YAML::Node node = YAML::LoadFile(path);

        if (node["status"]) {
            std::string status = node["status"].as<std::string>();
            if (status == "disabled") {
                throw std::runtime_error("Plugin " + path + " is disabled");
            }
        }
        plugin.name = node["name"] ? node["name"].as<std::string>() : "Unknown";
        plugin.path = path;
        plugin.checksum = FileCheckSum(binaryPath);
        plugin.entryPoint = node["entryPoint"] ? node["entryPoint"].as<std::string>() : "entry_point";
        YAML::Node deltaTimeNode = node["deltaTime"];
        if (deltaTimeNode.IsScalar()) {
            long tmp = deltaTimeNode.as<long>();
            plugin.deltaTime = tmp / 1000.0f;
        } else if (deltaTimeNode.IsMap()) {
            long tmp;
            if (eng::Engine::GetEngine()->IsClient()) {
                tmp = deltaTimeNode["client"] ? deltaTimeNode["client"].as<long>() : 0;
            } else if (eng::Engine::GetEngine()->IsServer()) {
                tmp = deltaTimeNode["server"] ? deltaTimeNode["server"].as<long>() : 0;
            }
            plugin.deltaTime = tmp / 1000.0f;
        }
        if (plugin.deltaTime < 0) {
            throw std::runtime_error("DeltaTime must be positive, milliseconds");
        }

        plugin.pipelinePosition = node["pipelinePosition"] ? node["pipelinePosition"].as<int>() : 0;

        plugin.handle = lib::LibUtils::getLibHandle(binaryPath);
        if (not plugin.handle) {
            throw std::runtime_error("Could not load plugin " + plugin.name + " at " + path);
        }

        if (node["hooks"]) {
            for (const auto& hook : node["hooks"].as<std::vector<std::string>>()) {
                void (*func)() = (void (*)())lib::LibUtils::getSymHandle(plugin.handle, hook);
                if (func == nullptr) {
                    throw std::runtime_error("Could not find hook " + hook + " in plugin " + plugin.name);
                }
                plugin.hooks[hook] = func;
            }
        }
        plugin.hooks[plugin.entryPoint] = (void (*)())lib::LibUtils::getSymHandle(plugin.handle, plugin.entryPoint);
        if (plugin.hooks[plugin.entryPoint] == nullptr) {
            throw std::runtime_error("Could not find entry point " + plugin.entryPoint + " in plugin " + plugin.name);
        }

        return plugin;
    }

    int ResourceManager::FileCheckSum(const std::string& path)
    {
        std::ifstream file(path);
        std::string line;
        int checksum = 0;

        if (!file.is_open())
            return 0;
        while (std::getline(file, line)) {
            for (char& c : line) {
                checksum *= 31;
                checksum += c;
            }
        }
        return checksum;
    }

    void ResourceManager::PluginLoadPipeline()
    {
        for (auto& plugin : _plugins) {
            int deltaTime = plugin.second.deltaTime;
            auto entryPoint = plugin.second.hooks[plugin.second.entryPoint];
            std::shared_ptr<eng::Timer> timer = std::make_shared<eng::Timer>();
            timer->Start();
            eng::Engine::GetEngine()->AddToPipeline([deltaTime, entryPoint, timer]() {
                if (timer->GetElapsedTime() > deltaTime) {
                    entryPoint();
                    timer->Restart();
                }
            },
                plugin.second.pipelinePosition, plugin.second.name);
        }
    }

    void ResourceManager::ReloadPlugins()
    {
        try {
            std::string path = eng::Engine::GetEngine()->GetConfigValue("pluginPath");

            for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".so") {
                    std::string pluginName = entry.path().stem().string();
                    if (pluginName.substr(0, 3) == "lib") {
                        pluginName = pluginName.substr(3);
                    }
                    std::string yamlFilePath = path + "/" + pluginName + "/" + pluginName + ".yaml";

                    if (_plugins.find(pluginName) != _plugins.end()) {
                        int checksum = FileCheckSum(entry.path().string());
                        if (checksum != _plugins[pluginName].checksum) {
                            CONSOLE::info << "Reloading plugin " << pluginName << std::endl;
                            lib::LibUtils::closeLibHandle(_plugins[pluginName].handle);
                            try {
                                _plugins[pluginName] = ParsePlugin(yamlFilePath, entry.path().string());
                            } catch (std::exception& e) {
                                CONSOLE::warn << e.what() << std::endl;
                                CONSOLE::warn << "Skipping plugin " << pluginName << std::endl;
                            }
                        }
                    } else {
                        CONSOLE::info << "Loading new plugin " << pluginName << std::endl;
                        _plugins[pluginName] = ParsePlugin(yamlFilePath, entry.path().string());
                    }
                }
            }
            PluginLoadPipeline();
        } catch (std::exception& e) {
            CONSOLE::warn << e.what() << std::endl;
        }
    }
}
