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
            dlclose(std::get<0>(_graphicalModule));
    }

    std::shared_ptr<AUserComponent> ResourceManager::LoadUserComponent(
        const std::string& resourceID)
    {
        std::cout << "Loading user component " << resourceID << std::endl;
        if (_handles.find(resourceID) != _handles.end()) {
            AUserComponent* (*create)() = reinterpret_cast<AUserComponent* (*)()>(dlsym(_handles[resourceID], "create"));
            if (!create)
                throw eng::EngineException(dlerror(), __FILE__, __FUNCTION__, __LINE__);
            _instances.emplace_back(create());
            return _instances.back();
        }

        std::string libName = m_userComponentsPath + "lib" + resourceID + ".so";
        void* handle = dlopen(libName.c_str(), RTLD_LAZY);

        if (!handle)
            throw eng::EngineException(dlerror(), __FILE__, __FUNCTION__, __LINE__);
        _handles[resourceID] = handle;
        AUserComponent* (*create)() = reinterpret_cast<AUserComponent* (*)()>(dlsym(handle, "create"));
        if (!create)
            throw eng::EngineException(dlerror(), __FILE__, __FUNCTION__, __LINE__);
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
            meta::MetadataGenerator().generateMetadata(copyPath);
        } catch (std::exception& e) {
            Console::err << "Build failed: " << path << std::endl;
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
                Console::info << "Checking " << entry.path() << std::endl;
                if (entry.path().extension() == ".cpp") {
                    ManageUpdate(entry.path().string());
                }
            }
            if (m_changesNbr == 0)
                Console::info << "No changes detected in user scripts" << std::endl;
        } catch (std::runtime_error& e) {
            Console::err << "Compilation error during hot reload : " << e.what() << std::endl;
            Console::err << "Aborting" << std::endl;
            return;
        }
        if (m_changesNbr > 0)
            Sys.ReloadEntities();
    }

    void ResourceManager::ManageUpdate(const std::string& path)
    {
        if (FileHasChanged(path)) {
            ++m_changesNbr;
            Console::info << "Detected changes in source file " << path << std::endl;
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
            dlclose(handle.second);
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

        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            throw eng::EngineException(dlerror(), __FILE__, __FUNCTION__, __LINE__);
        }
        graph::IGraphicalModule* (*create)() = reinterpret_cast<graph::IGraphicalModule* (*)()>(dlsym(handle, "create"));
        if (!create) {
            throw eng::EngineException(dlerror(), __FILE__, __FUNCTION__, __LINE__);
        }
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
}
