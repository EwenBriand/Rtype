/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ResourceManager.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IGame.hpp"
#include "IGraphicalModule.hpp"
#include "Types.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

// typedef HINSTANCE__ *HINSTANCE;
// typedef HINSTANCE HMODULE;
// typedef long long INT_PTR
// typedef INT_PTR (__stdcall *FARPROC)()
// #include <windows.h>

namespace ecs {
    class ResourceManager {
    public:
        struct PluginInfo {
            std::string name;
            std::string path;
            int checksum;
            std::string entryPoint;
            float deltaTime;
            int pipelinePosition;
            std::map<std::string, void (*)()> hooks;
            void* handle;
        };

        ~ResourceManager();

        /**
         * @brief Returns an instance of the AUserComponent child class
         * contained in the shared library with the name given in parameter.
         * The library should contain a method named create, which
         * will return a pointer to the instance of the class. This method can
         * be generated easily using the MANAGED_RESOURCE macro in metadata.hpp.
         *
         * @param resourceID : the name of the library, minus lib and .so.
         * For example, if the library is named libTest.so, the resourceID
         * is Test, which should also be the name of the class contained in
         * the library.
         * @return AUserComponent*
         */
        std::shared_ptr<AUserComponent> LoadUserComponent(const std::string& resourceID);

        /**
         * @brief Detect all the user scripts and
         * checks if they have changed by comparing a old checksum of the file
         * with the new one. If the checksum is different, the library is
         * recompiled, reloaded, and the ECS is reloaded if the compilation
         * was successful.
         *
         */
        void CheckHotReload();

        /**
         * @brief Compiles the user script given in parameter, and generate
         * optional metadata for it. Returns true if the compilation was
         * successful, false otherwise.
         *
         */
        bool CompileUserScript(const std::string& path);

        /**
         * @brief Returns the checksum of the file given in parameter.
         *
         */
        std::string GetChecksum(const std::string& path);

        /**
         * @brief Returns true if the file has changed.
         * Only tests .cpp files, and also checks for the corresponding hpp file.
         *
         */
        bool FileHasChanged(const std::string& path);

        /**
         * @brief Manages the changes in the user scripts.
         *
         */
        void ManageUpdate(const std::string& path);

        /**
         * @brief Clears all handles. Make sure that every reference to
         * an object that originates from a shared library is deleted before
         * calling this function.
         *
         */
        void Clear();

        /**
         * @brief Returns the list of the names of all the registered actions.
         * An action is a function pointer that can be called, usually declared
         * as a lambda expression.
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> GetActionList() const;

        /**
         * @brief Registers a new action to render it available to the
         * rest of the code.
         *
         */
        void RegisterPublicAction(const std::string& name, Action action);

        /**
         * @brief Loads a graphical module from the given file path.
         * If the path is empty, the default module ( graphical ray lib )
         * is loaded.
         *
         */
        std::shared_ptr<graph::IGraphicalModule> LoadGraphicalModule(const std::string& path = "");

        /**
         * @brief Loads a text file from the asset folder and returns its contents as
         * a null terminated string.
         *
         */
        std::string LoadFileText(const std::string& path);

        /**
         * @brief Returns the path composed of the elements in the vector.
         *
         */
        static std::string MakePath(const std::vector<std::string>& pathElements, bool throwIfNotExist = false);

        /**
         * @brief Loads an instance of the IGame interface from the given file path.
         *
         */
        std::shared_ptr<eng::IGame> LoadGame(const std::string& path);

        /**
         * @brief Loads a prefab from its name containing the save of an entity, from the ${assetRoot}/prefabs folder.
         * It returns a handle to the entity instantiated, or -1 if the instantiation failed.
         *
         */
        int LoadPrefab(const std::string& name);

        /**
         * @brief Saves an entity as a prefab in the ${assetRoot}/prefabs folder.
         *
         */
        void SavePrefab(const std::string& name, int entity);

        /**
         * @brief HotLoads the given library. (meaning it will try to compile it before loading it)
         *
         *  @param path : the name of the .cpp file containing the code of the library.
         *  This file should be in the userScriptsPath defined in base.cfg.
         */
        void HotLoadLibrary(const std::string& path);

        void LoadPlugins();

        void discoverPlugins(const std::string& path);

        PluginInfo ParsePlugin(const std::string& path, const std::string& binaryPath);

        int FileCheckSum(const std::string& path);

        void PluginLoadPipeline();

        void ReloadPlugins();

    private:
        unsigned int m_changesNbr = 0;
        std::string m_userComponentsPath = "./metabuild/";
        std::vector<std::shared_ptr<AUserComponent>> _instances;
        std::unordered_map<std::string, void*> _handles;

        std::unordered_map<std::string, Action> _availableActions;
        std::tuple<void*, std::shared_ptr<graph::IGraphicalModule>> _graphicalModule = { nullptr, nullptr };
        std::shared_ptr<eng::IGame> _game = nullptr;
        void* _gameHandle = nullptr;
        std::map<std::string, PluginInfo> _plugins;
    };
}
