/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ResourceManager.cpp
*/

// /*
// ** EPITECH PROJECT, 2023
// ** ia
// ** File description:
// ** LibLoader.hpp
// */

// #pragma once

// #include <map>
// // if windows
// #ifdef _WIN32
// #include <windows.h>
// #else
// #include <dlfcn.h>
// #endif
// #include <string>
// #include <memory>
// #include <iostream>
// #include <stdexcept>

// #define LOADABLE(class) \
//     extern "C" std::shared_ptr<class> create() { \
//         return std::make_shared<class>(); \
//     }

// namespace ai {
//     class LibLoader {
//         public:
//             ~LibLoader();
//             void LoadLib(const std::string &path);
//             void UnloadLib(const std::string &path);

// #ifdef _WIN32
//             template <typename T>
//             std::shared_ptr<T> GetLib(const std::string &path) {
//                 HMODULE hModule = LoadLibrary(path.c_str());
//                 if (hModule == NULL) {
//                     throw std::runtime_error("Failed to load library");
//                 }
//                 FARPROC functionPointer = GetProcAddress(hModule, "create");
//                 if (functionPointer == NULL) {
//                     LibUtils::closeLibHandlerary(hModule);
//                     throw std::runtime_error("Failed to get function pointer");
//                 }
//                 typedef std::shared_ptr<T> (*CreateFunctionType)();
//                 CreateFunctionType createFunction = reinterpret_cast<CreateFunctionType>(functionPointer);
//                 std::shared_ptr<T> result = createFunction();
//                 _libs[path] = hModule;
//                 return result;
//             }

// #else
//             template <typename T>
//             std::shared_ptr<T> GetLib(const std::string &path) {
//                 if (_libs.find(path) == _libs.end())
//                     LoadLib(path);
//                 void *getter = GETSYMBOL(_libs[path], "create");
//                 if (!getter)
//                     throw std::runtime_error(ERRORLIB());
//                 return reinterpret_cast<std::shared_ptr<T>(*)()>(getter)();
//             }
// #endif
//         private:
//             std::map<std::string, void *> _libs;
//     };
// }

#include "ResourceManager.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GraphicalRayLib/GraphicalRayLib.hpp"
#include "IGraphicalModule.hpp"
#include "LibUtils.hpp"
#include "metadataGenerator.hpp"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <type_traits>
#include <unistd.h>

namespace ecs {
    ResourceManager::~ResourceManager()
    {
        if (std::get<0>(_graphicalModule))
            lib::LibUtils::closeLibHandle(std::get<0>(_graphicalModule));
        if (_gameHandle)
            lib::LibUtils::closeLibHandle(_gameHandle);
    }

    std::shared_ptr<AUserComponent> ResourceManager::LoadUserComponent(
        const std::string& resourceID)
    {
        std::cout << "Loading user component " << resourceID << std::endl;
        if (_handles.find(resourceID) != _handles.end()) {
            AUserComponent* (*create)() = reinterpret_cast<AUserComponent* (*)()>(lib::LibUtils::getSymHandle(_handles[resourceID], "create"));
            _instances.emplace_back(create());
            return _instances.back();
        }

        std::string libName = m_userComponentsPath + "lib" + resourceID + ".so";
        void* handle = lib::LibUtils::getLibHandle(libName.c_str());
        _handles[resourceID] = handle;
        AUserComponent* (*create)() = reinterpret_cast<AUserComponent* (*)()>(lib::LibUtils::getSymHandle(handle, "create"));
        _handles[resourceID] = handle;
        _instances.emplace_back(create());
        _instances.back()->OnLoad();
        return _instances.back();
    }

    bool ResourceManager::CompileUserScript(const std::string& path)
    {
        std::string tmpCopyDirectory = ".tmpCompileSrc";
        std::string rawPath = path.substr(0, path.find_last_of('.'));
        char buff[1024] = { 0 };
        getcwd(buff, 1024);
        std::string copyPath = std::string(buff) + tmpCopyDirectory + "/" + rawPath.substr(rawPath.find_last_of('/') + 1);
        std::string command = "mkdir -p " + copyPath + " && cp " + rawPath + ".cpp " + rawPath + ".hpp " + copyPath;
        system(command.c_str());
        try {
            auto metagen = meta::MetadataGenerator();
            metagen.generateMetadata(copyPath);
            metagen.buildCMake();
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
                CONSOLE::info << "Checking " << entry.path() << std::endl;
                if (entry.path().extension() == ".cpp") {
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

        if (path == "") {
            auto tmp = std::shared_ptr<graph::IGraphicalModule>(new raylib::GraphicalRayLib());
            _graphicalModule = std::make_tuple(nullptr, tmp);
            return std::get<1>(_graphicalModule);
        }

        void* handle = lib::LibUtils::getLibHandle(path.c_str());
        graph::IGraphicalModule* (*create)() = reinterpret_cast<graph::IGraphicalModule* (*)()>(lib::LibUtils::getSymHandle(handle, "create"));
        auto tmp = std::shared_ptr<graph::IGraphicalModule>(create());
        if (!std::get<1>(_graphicalModule)) {
            throw eng::EngineException("Graphical module creation failed", __FILE__, __FUNCTION__, __LINE__);
        }
        _graphicalModule = std::make_tuple(handle, tmp);
        return std::get<1>(_graphicalModule);
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
        eng::IGame* (*create)() = reinterpret_cast<eng::IGame* (*)()>(lib::LibUtils::getSymHandle(handle, "create"));
        _game = std::shared_ptr<eng::IGame>(create());
        if (!_game) {
            throw eng::EngineException("Game creation failed", __FILE__, __FUNCTION__, __LINE__);
        }
        _gameHandle = handle;
        return _game;
    }
}
