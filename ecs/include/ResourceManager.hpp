/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ResourceManager.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IGraphicalModule.hpp"
#include "Types.hpp"
#include <cstring>
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace ecs {
    class ResourceManager {
    public:
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

    private:
        unsigned int m_changesNbr = 0;
        std::string m_userComponentsPath = "./metabuild/";
        std::vector<std::shared_ptr<AUserComponent>> _instances;
        std::unordered_map<std::string, void*> _handles;

        std::unordered_map<std::string, Action> _availableActions;
        std::tuple<void*, std::shared_ptr<graph::IGraphicalModule>> _graphicalModule = { nullptr, nullptr };
    };
}
