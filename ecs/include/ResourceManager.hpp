/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ResourceManager.hpp
*/

#pragma once

#include <iostream>
#include <dlfcn.h>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <cstring>
#include <type_traits>
#include <iterator>
#include <unordered_map>
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Types.hpp"
#include "IGraphicalModule.hpp"

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
            std::shared_ptr<AUserComponent> LoadUserComponent(const std::string &resourceID);

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
            bool CompileUserScript(const std::string &path);

            /**
             * @brief Returns the checksum of the file given in parameter.
             *
             */
            std::string GetChecksum(const std::string &path);

            /**
             * @brief Returns true if the file has changed.
             * Only tests .cpp files, and also checks for the corresponding hpp file.
             *
             */
            bool FileHasChanged(const std::string &path);

            /**
             * @brief Manages the changes in the user scripts.
             *
             */
            void ManageUpdate(const std::string &path);

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
            void RegisterPublicAction(const std::string &name, Action action);

            /**
             * @brief Loads a graphical module from the given file path.
             * If the path is empty, the default module ( graphical ray lib )
             * is loaded.
             *
             */
            std::shared_ptr<graph::IGraphicalModule> LoadGraphicalModule(const std::string &path = "");

            /**
             * @brief Loads a text file from the asset folder and returns its contents as
             * a null terminated string.
             *
             */
            std::string LoadFileText(const std::string &path);

            template<typename T, typename = void>
            struct is_iterable : std::false_type {};

            template<typename T>
            struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type {};

            template <typename T>
            static std::enable_if_t<!is_iterable<T>::value> Serialize(const T &object, std::vector<unsigned char> &data)
            {
                std::vector<unsigned char> tmp;
                tmp.resize(sizeof(T));
                memcpy(tmp.data(), &object, sizeof(T));
                data.insert(data.end(), tmp.begin(), tmp.end());
            }

            template <typename T>
            static std::enable_if_t<is_iterable<T>::value> Serialize(const T &object, std::vector<unsigned char> &data)
            {
                Serialize(object.size(), data);
                for (const auto &item : object)
                {
                    Serialize(item, data);
                }
            }

            template <typename T>
            static std::enable_if_t<!is_iterable<T>::value> Deserialize(const std::vector<unsigned char> &data, std::vector<unsigned char>::iterator &it, T &object)
            {
                memcpy(&object, &(*it), sizeof(T));
                it += sizeof(T);
            }

            template <typename T>
            static std::enable_if_t<is_iterable<T>::value> Deserialize(const std::vector<unsigned char> &data, std::vector<unsigned char>::iterator &it, T &object)
            {
                size_t size;
                Deserialize(data, it, size);
                object.resize(size);
                for (size_t i = 0; i < size; ++i)
                {
                    typename T::value_type item;
                    Deserialize(data, it, item);
                    object[i] = item;
                }
            }

        private:
            unsigned int m_changesNbr = 0;
            std::string m_userComponentsPath = "./metabuild/";
            std::vector<std::shared_ptr<AUserComponent>> _instances;
            std::unordered_map<std::string, void *> _handles;

            std::unordered_map<std::string, Action> _availableActions;
            std::tuple<void *, std::shared_ptr<graph::IGraphicalModule>> _graphicalModule = {nullptr, nullptr};
    };
}
