/*
** EPITECH PROJECT, 2022
** ecs
** File description:
** ECS.hpp
*/

#pragma once

#include "Component.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "Components.Vanilla/Tag.hpp"
#include "ECSImpl.hpp"
#include "Entity.hpp"
#include "IGraphicalModule.hpp"
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <variant>
#include <vector>

#define GetUComponent(e, typename) \
    dynamic_cast<typename*>(SYS.GetComponent(e, #typename).GetInternalComponent().get())

namespace ecs {
    static const std::string red = "\033[0;31m";
    static const std::string green = "\033[0;32m";
    static const std::string yellow = "\033[0;33m";
    static const std::string blue = "\033[0;34m";
    static const std::string magenta = "\033[0;35m";
    static const std::string cyan = "\033[0;36m";
    static const std::string reset = "\033[0m";
    static const std::string white = "\033[0m";

    template <int... N>
    struct sequence { };

    template <int N, int... S>
    struct gen_seq : gen_seq<N - 1, N - 1, S...> { };

    template <int... S>
    struct gen_seq<0, S...> {
        typedef sequence<S...> type;
    };

    template <typename... VanillaComponents>
    class ECS {
    public:
        using AnyCpt = std::variant<VanillaComponents...>;
        using AnyMultiCpt = std::vector<AnyCpt>;
        using AllCptOfSameType = std::vector<AnyMultiCpt>;
        using AllCpt = std::array<AllCptOfSameType, sizeof...(VanillaComponents)>;

        typedef enum {
            LOGNOPE,
            LOGINFO,
            LOGWARNING,
            LOGERROR
        } loglevel_t;

        static loglevel_t LogLevel;

        struct WarningStream {
            template <typename T>
            WarningStream& operator<<(const T& t)
            {
                if (LogLevel > LOGWARNING)
                    return *this;
                std::cout << yellow << t << reset;
                return *this;
            }

            WarningStream& operator<<(std::ostream& (*f)(std::ostream&))
            {
                if (LogLevel > LOGWARNING)
                    return *this;
                std::cout << f;
                return *this;
            }
        };

        struct ErrorStream {
            template <typename T>
            ErrorStream& operator<<(const T& t)
            {
                if (LogLevel > LOGERROR)
                    return *this;
                std::cerr << red << t << reset;
                return *this;
            }

            ErrorStream& operator<<(std::ostream& (*f)(std::ostream&))
            {
                if (LogLevel > LOGERROR)
                    return *this;
                std::cerr << f;
                return *this;
            }
        };

        struct InfoStream {
            template <typename T>
            InfoStream& operator<<(const T& t)
            {
                if (LogLevel > LOGINFO)
                    return *this;
                std::cerr << green << t << reset;
                return *this;
            }

            InfoStream& operator<<(std::ostream& (*f)(std::ostream&))
            {
                if (LogLevel > LOGINFO)
                    return *this;
                std::cout << f;
                return *this;
            }
        };

        struct PromptStream {
            template <typename T>
            PromptStream& operator<<(const T& t)
            {
                std::cout << cyan << t << reset;
                return *this;
            }

            PromptStream& operator<<(std::ostream& (*f)(std::ostream&))
            {
                std::cout << f;
                return *this;
            }

            PromptStream& operator>>(std::string& str)
            {
                std::cin >> str;
                return *this;
            }
        };

        static WarningStream warn;
        static ErrorStream err;
        static InfoStream info;
        static PromptStream prompt;

        static ECS<VanillaComponents...>& GetSystem()
        {
            if (System == nullptr)
                System = std::make_unique<ECS<VanillaComponents...>>();
            return *(System.get());
        }

        static std::unique_ptr<ECS<VanillaComponents...>> System;

        ECS()
        {
            initializeComponentIndexes(typename gen_seq<sizeof...(VanillaComponents)>::type());
            _systemHolder = RegisterEntity();
        }

        /**
         * @brief Initializes the UI.
         *
         */
        void InitUI();

        /**
         * @brief Registers an entity in the ECS.
         *
         * @return int The ID of the entity.
         */
        Entity RegisterEntity()
        {
            if (_freeIds.empty()) {
                for (auto& cpt : _components)
                    cpt.emplace_back();
                _usedIds.push_back(_components[0].size() - 1);
                return _components[0].size() - 1;
            }
            int id = _freeIds.front();
            _freeIds.pop();
            _usedIds.push_back(id);
            return id;
        }

        /**
         * @brief Unregisters an entity from the ECS.
         *
         * @param e The entity to unregister.
         */
        void UnregisterEntity(Entity e)
        {
            if (e >= _components[0].size())
                throw std::runtime_error("Entity ID out of range");
            if (std::find(_usedIds.begin(), _usedIds.end(), e) == _usedIds.end())
                throw std::runtime_error("Entity ID not registered");
            for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                _components[i][e].clear();
            }
            _usedIds.erase(std::remove(_usedIds.begin(), _usedIds.end(), e), _usedIds.end());
            _freeIds.push(e);
            _deletedThisFrame.push(e);
            _editorEntityContext = _systemHolder;
        }

        /**
         * @brief Adds a component to an entity. Can only be used with vanilla components.
         *
         * @tparam T
         * @param e
         * @return T&
         */
        template <typename T>
        T& AddComponent(Entity e)
        {
            size_t idx = _cptTypesIndexes[std::type_index(typeid(T))];

            _components[idx][e].emplace_back(T());
            std::visit([&](auto&& arg) {
                arg.OnAddComponent(e);
            },
                _components[idx][e].back());
            return std::get<T>(_components[idx][e].back());
        }

        /**
         * @brief Get the component type if find else create it
         *
         * @tparam T
         * @param value, client  element to catch and the client
         * @return T&
         */

        template <typename T>
        T& SafeGet(CLI& cli)
        {
            T* result = nullptr;
            if (cli.GetContext() == GetSystemHolder()) {
                throw std::runtime_error("Can't add " + std::string(typeid(T).name()) + " to the system holder");
            }
            try {
                result = &GetComponent<T>(cli.GetContext());
            } catch (std::exception& e) {
                AddComponent<T>(cli.GetContext());
                result = &GetComponent<T>(cli.GetContext());
            }
            if (result == nullptr)
                throw std::runtime_error("Can't add " + std::string(typeid(T).name()) + " to the system holder");
            return *result;
        }

        template <typename T>
        T& SafeGet(Entity e)
        {
            T* result = nullptr;
            if (e == GetSystemHolder()) {
                throw std::runtime_error("Can't add " + std::string(typeid(T).name()) + " to the system holder");
            }
            try {
                result = &GetComponent<T>(e);
            } catch (std::exception& except) {
                result = &AddComponent<T>(e);
            }
            if (result == nullptr)
                throw std::runtime_error("Can't add " + std::string(typeid(T).name()) + " to the system holder");
            return *result;
        }

        /**
         * @brief Adds a component loaded dynamically from a shared library.
         *
         * @param e
         * @param cptTypeName
         */
        UserComponentWrapper& AddComponent(Entity e, const std::string& cptTypeName)
        {
            try {
                UserComponentWrapper& wrapper = AddComponent<UserComponentWrapper>(e);
                std::shared_ptr<AUserComponent> internal = _resourceManager.LoadUserComponent(cptTypeName);
                internal->OnAddComponent(e);
                wrapper.SetInternalComponent(internal);
                wrapper.SetResourceID(cptTypeName);
                return wrapper;
            } catch (std::exception& ex) {
                std::cout << red << "Failed to load component: " << ex.what() << std::endl;
                std::cout << yellow << "If you think this is because the binary has not been rebuilt, try manually deleting the " << cptTypeName << ".checksum file associated with the component." << std::endl;
                throw ex;
            }
        }

        template <typename T>
        T& GetComponent(Entity e, const std::string& cptTypeName)
        {
            std::type_index type = std::type_index(typeid(T));
            size_t idx = 0;

            try {
                idx = _cptTypesIndexes.at(type);
            } catch (std::exception& e) {
                idx = _cptTypesIndexes.at(std::type_index(typeid(UserComponentWrapper)));
            }

            bool found = false;
            for (auto& cpt : _components[idx][e]) {
                std::visit([&](auto&& arg) {
                    if (arg.GetClassName() == cptTypeName) {
                        found = true;
                    }
                },
                    cpt);
                if (found) {
                    T* tmp = dynamic_cast<T*>(std::get<UserComponentWrapper>(cpt).GetInternalComponent().get());
                    if (tmp == nullptr)
                        throw std::runtime_error("Component found uninitialized : " + cptTypeName);
                    return *tmp;
                }
            }
            throw std::runtime_error("Component not found : " + cptTypeName);
        }

        /**
         * @brief Assigns a pre existing component to an entity.
         *
         * @tparam T
         * @param e
         * @param Cpt
         * @return T&
         */
        template <typename T>
        T& AssignComponent(Entity e, T& Cpt)
        {
            size_t idx = _cptTypesIndexes[std::type_index(typeid(T))];

            _components[idx][e].push_back(Cpt);
            // _components[idx][e].back().OnAddComponent(e);
            std::visit([&](auto&& arg) {
                arg.OnAddComponent(e);
            },
                _components[idx][e].back());
            return std::get<T>(_components[idx][e].back());
        }
        /**
         * @brief Returns a component of the given type from an entity.
         *
         * @tparam T
         * @param e
         */
        template <typename T>
        T& GetComponent(Entity e)
        {
            size_t idx = _cptTypesIndexes[std::type_index(typeid(T))];
            if (_components[idx].size() < e)
                throw std::runtime_error("Entity not found : " + std::string(typeid(T).name()));
            if (_components[idx][e].empty())
                throw std::runtime_error("Component not found : " + std::string(typeid(T).name()));

            return std::get<T>(_components[idx][e].back());
        }

        /**
         * @brief Returns a vector of all the components of the given type
         * for a given entity.
         *
         * @tparam T
         * @return std::vector<T &>
         */
        template <typename T>
        std::vector<T&> GetComponents(Entity e)
        {
            size_t idx = _cptTypesIndexes[std::type_index(typeid(T))];
            std::vector<T&> res;

            for (auto& cpt : _components[idx][e]) {
                res.push_back(std::get<T>(cpt));
            }
            return res;
        }

        std::vector<AnyCpt> GetComponentsByName(Entity e, std::string name)
        {
            std::vector<AnyCpt> v;
            size_t idx = 0;

            for (; idx < cloneBase.size(); ++idx) {
                bool breakHere = false;

                std::visit([&](auto&& arg) {
                    if (arg.GetClassName() == name) {
                        breakHere = true;
                    }
                },
                    cloneBase[idx]);
                if (breakHere)
                    break;
            }
            if (idx == cloneBase.size())
                throw std::runtime_error("Component not found : " + name);
            for (auto& cpt : _components[idx][e]) {
                v.push_back(cpt);
            }
            return v;
        }

        /**
         * @brief Adds a component from its index in the ECS's template definition.
         * This only works for components that are defined in the ECS's template,
         * and not for user defined components.
         *
         */
        void AddVanillaComponentFromIndex(Entity e, size_t idx)
        {
            if (idx >= sizeof...(VanillaComponents))
                throw std::runtime_error("Component index out of range");
            _components[idx][e].emplace_back(cloneBase[idx]);
            // _components[idx][e].back().OnAddComponent(e);
            std::visit([&](auto&& arg) {
                arg.OnAddComponent(e);
            },
                _components[idx][e].back());
        }

        /**
         * @brief Adds a component from the name of its type.
         *
         * @param e
         * @param name
         */
        void AddVanillaComponentFromName(Entity e, const std::string& name)
        {
            size_t idx = 0;
            bool found = false;

            for (auto& cpt : cloneBase) {
                std::visit([&](auto&& arg) {
                    if (arg.GetClassName() == name) {
                        found = true;
                        _components[idx][e].emplace_back(arg);
                        std::visit([&](auto&& arg) {
                            arg.OnAddComponent(e);
                        },
                            _components[idx][e].back());
                    }
                },
                    cpt);
                if (found)
                    return;
                ++idx;
            }
            throw std::runtime_error("Component not found : " + name);
        }

        /**
         * @brief Sets the value of a component's member from the name
         * of the member. This only works for components that provide accurate metadata.
         * As the value is given as a string, only basic types are supported.
         *
         */
        void SetMember(Entity e, int cptIdx, const std::string& memberName, const std::string& value)
        {
            int totalElementsYet = 0;
            int idx = cptIdx;
            int i = 0;

            if (cptIdx < 0)
                throw std::runtime_error("Component index out of range");
            while (totalElementsYet <= cptIdx && i < sizeof...(VanillaComponents)) {
                totalElementsYet += _components[i][e].size();
                ++i;
            }
            --i;
            if (totalElementsYet < cptIdx)
                throw std::runtime_error("Component index out of range");
            idx = cptIdx - (totalElementsYet - _components[i][e].size());
            std::visit([&](auto&& arg) {
                try {
                    metadata_t metadata = arg.GetMetadata().at(memberName);
                    metadata.set(value);
                } catch (std::exception& e) {
                    throw std::runtime_error("SetMember: Invalid member name");
                }
            },
                _components[i][e][idx]);
        }

        std::map<std::string, metadata_t> GetMetadataFromIndex(Entity e, int cptIdx)
        {
            int totalElementsYet = 0;
            int idx = cptIdx;
            int i = 0;

            if (cptIdx < 0)
                throw std::runtime_error("Component index out of range");
            while (totalElementsYet <= cptIdx && i < sizeof...(VanillaComponents)) {
                totalElementsYet += _components[i][e].size();
                ++i;
            }
            --i;
            if (totalElementsYet < cptIdx)
                throw std::runtime_error("Component index out of range");
            idx = cptIdx - (totalElementsYet - _components[i][e].size());
            std::map<std::string, metadata_t> metadata = std::visit([&](auto&& arg) {
                return arg.GetMetadata();
            },
                _components[i][e][idx]);
            return metadata;
        }

        /**
         * @brief CallStart calls the Start() method on all registered components.
         * It is called by the ECS when the game starts.
         *
         */
        void CallStart()
        {
            for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                AllCptOfSameType& cpt = _components[i];
                for (Entity& e : _usedIds) {
                    if (e < 0 || cpt.size() <= (unsigned int)e)
                        continue;
                    for (AnyCpt& cpt : cpt[e]) {
                        std::visit([&](auto&& arg) {
                            arg.Start();
                        },
                            cpt);
                    }
                }
            }
        }

        /**
         * @brief calls the OnLoad function on all the vanilla components
         * managed by the ECS. Concretely, the instance of the component
         * in cloneBase has its OnLoad function called.
         *
         */
        void LoadVanilla()
        {
            for (auto& cpt : cloneBase) {
                std::visit([&](auto&& arg) {
                    arg.OnLoad();
                    std::cout << green << "Loaded component " << arg.GetClassName() << "\n";
                },
                    cpt);
            }
            std::cout << white << std::endl;
        }

        /**
         * @brief Calls the UpdateImpl function of all the components.
         *
         */
        void Update()
        {
            updateHelper(typename gen_seq<sizeof...(VanillaComponents)>::type());
            while (!_deletedThisFrame.empty()) {
                Entity e = _deletedThisFrame.front();
                for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                    _components[i][e].clear();
                }
                _deletedThisFrame.pop();
            }
        }

        template <int... N>
        void updateHelper(sequence<N...>)
        {
            if (_skipFrame)
                return;
            (udateImpl<N>(), ...);
        }

        template <int Idx>
        void udateImpl()
        {
            AllCptOfSameType& cpt = _components[Idx];
            auto currUsedIds = _usedIds;
            for (Entity& e : currUsedIds) {
                if (e < 0)
                    continue;
                for (auto& cpt : cpt[e]) {
                    std::visit([&](auto&& arg) {
                        if (_skipFrame)
                            return;
                        arg.Update(e);
                    },
                        cpt);
                }
            }
        }

        /**
         * @brief Sets the save path for the ECS.
         *
         */
        void SetSavePath(const std::string& path)
        {
            m_savePath = path;
        }

        /**
         * @brief Returns the save path of the ECS.
         *
         */
        std::string GetSavePath() const
        {
            return m_savePath;
        }

        /**
         * @brief Saves the entity to a directory named after its ID.
         * The directory has one file per component, numbered from 0 to
         * the number of components of the entity.
         * The file is named after the component's index. The save function
         * of the component is called to save the component.
         *
         * @param e : the entity to save
         * @param path : an optional path to save the entity to. If not specified,
         * the entity is saved to the save directory.
         */
        void SaveEntity(Entity e, const std::string& path = "")
        {
            try {
                if (GetComponent<Tag>(e).Get() == "no save")
                    return;
            } catch (std::exception& e) {
            }
            std::string thisPath = "";

            if (path == "") {
                std::filesystem::create_directory(m_savePath);
                thisPath = m_savePath + "/" + std::to_string(e);
            } else {
                thisPath = path;
            }
            std::filesystem::create_directory(thisPath);
            std::vector<std::string> cptTypes;

            for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                AllCptOfSameType& cpt = _components[i];
                if (cpt.size() <= (unsigned int)e)
                    continue;
                for (size_t j = 0; j < cpt[e].size(); ++j) {
                    std::visit([&](auto&& arg) {
                        arg.Save(thisPath + "/" + std::to_string(i) + "_" + std::to_string(j));
                        if (typeid(arg) == typeid(UserComponentWrapper)) {
                            cptTypes.push_back("UserComponentWrapper");
                        } else {
                            cptTypes.push_back(arg.GetClassName());
                        }
                    },
                        cpt[e][j]);
                }
            }
            std::ofstream file(thisPath + "/cptTypes");
            for (auto& cptType : cptTypes)
                file << cptType << std::endl;
            file.close();
        }

        /**
         * @brief Loads an entity from the given folder.
         *
         * @return Entity : the ID of the loaded entity.
         */
        Entity LoadEntity(const std::string& path)
        {
            if (path.substr(path.find_last_of("/"), path.size()) == std::to_string(_systemHolder))
                return -1;
            Entity e = RegisterEntity();
            std::ifstream cptTypes(path + "/cptTypes");
            std::string type = "";

            std::vector<std::string> filesAlphabetical;
            for (auto& p : std::filesystem::directory_iterator(path)) {
                filesAlphabetical.push_back(p.path().string());
            }
            std::sort(filesAlphabetical.begin(), filesAlphabetical.end(), [](const std::string& a, const std::string& b) {
                std::string aNumStr = a.substr(a.find_last_of("/") + 1, a.find_first_of("_") - a.find_last_of("/") - 1);
                std::string bNumStr = b.substr(b.find_last_of("/") + 1, b.find_first_of("_") - b.find_last_of("/") - 1);
                if (a.find("cptTypes") != std::string::npos || a.find("_internal") != std::string::npos) {
                    return false;
                } else if (b.find("cptTypes") != std::string::npos || b.find("_internal") != std::string::npos) {
                    return true;
                }
                return std::stoi(aNumStr) < std::stoi(bNumStr);
            });

            for (auto& p : filesAlphabetical) {
                if (p.substr(p.find_last_of("/"), p.size()) == "/cptTypes")
                    continue;
                if (p.substr(p.find_last_of("_"), p.size()) == "_internal")
                    continue;
                std::getline(cptTypes, type);

                for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                    std::visit([&](auto&& arg) {
                        if (arg.GetClassName() == type) {
                            _components[i][e].emplace_back(arg);
                            std::visit([&](auto&& tmp) {
                                tmp.Load(p);
                                tmp.OnAddComponent(e);
                            },
                                _components[i][e].back());
                        }
                    },
                        cloneBase[i]);
                }
            }
            cptTypes.close();
            return e;
        }

        /**
         * @brief Performs the same as LoadEntity, but instead of registering
         * it to the ecs, it dumps it into the given storage.
         *
         * @param path
         * @param storage
         */
        void DumpEntity(const std::string& path, AllCpt& storage)
        {
            if (path.substr(path.find_last_of("/"), path.size()) == std::to_string(_systemHolder))
                return;
            std::ifstream cptTypes(path + "/cptTypes");
            std::string type = "";

            std::vector<std::string> filesAlphabetical;
            for (auto& p : std::filesystem::directory_iterator(path)) {
                filesAlphabetical.push_back(p.path().string());
            }
            std::sort(filesAlphabetical.begin(), filesAlphabetical.end(), [](const std::string& a, const std::string& b) {
                std::string aNumStr = a.substr(a.find_last_of("/") + 1, a.find_first_of("_") - a.find_last_of("/") - 1);
                std::string bNumStr = b.substr(b.find_last_of("/") + 1, b.find_first_of("_") - b.find_last_of("/") - 1);
                if (a.find("cptTypes") != std::string::npos || a.find("_internal") != std::string::npos) {
                    return false;
                } else if (b.find("cptTypes") != std::string::npos || b.find("_internal") != std::string::npos) {
                    return true;
                }
                return std::stoi(aNumStr) < std::stoi(bNumStr);
            });
            for (auto& cptCol : storage) {
                cptCol.emplace_back();
            }

            for (auto& p : filesAlphabetical) {
                if (p.substr(p.find_last_of("/"), p.size()) == "/cptTypes")
                    continue;
                if (p.substr(p.find_last_of("_"), p.size()) == "_internal")
                    continue;
                std::getline(cptTypes, type);
                for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                    std::visit([&](auto&& arg) {
                        if (arg.GetClassName() == type) {
                            storage[i][storage[i].size() - 1].emplace_back(arg);
                            std::visit([&](auto&& tmp) {
                                tmp.Load(p);
                                // tmp.OnAddComponent(storage[i].size() - 1);
                            },
                                storage[i][storage[i].size() - 1].back());
                        }
                    },
                        cloneBase[i]);
                }
            }
        }

        /**
         * @brief Clears all entities and reloads them from the
         * save directory.
         *
         * @return std::vector<Entity>
         */
        void ReloadEntities()
        {
            std::vector<Entity> entities = GetEntities();

            RequestEngineClearPipeline();
            for (auto& e : entities) {
                if (e == _systemHolder)
                    continue;
                for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                    _components[i][e].clear();
                }
                _usedIds.erase(std::remove(_usedIds.begin(), _usedIds.end(), e), _usedIds.end());
                _freeIds.push(e);
            }
            _skipFrame = true;
            _deletedThisFrame = std::queue<int>();
            _resourceManager.Clear();
            _resourceManager.CheckHotReload();
            for (auto& p : std::filesystem::directory_iterator(m_savePath)) {
                if (p.path().filename() == std::to_string(_systemHolder))
                    continue;
                LoadEntity(p.path().string());
            }
            _editorEntityContext = _systemHolder;
            for (auto& cpt : cloneBase) {
                std::visit([&](auto&& arg) {
                    arg.OnLoad();
                },
                    cpt);
            }
            NotifyEnginePipelineErased();
        }

        /**
         * @brief Replace Scene does the same as reloadEntities but it does not
         * load the entities, rather it takes them in argument.
         *
         */
        void ReplaceScene(AllCpt& storage)
        {
            std::vector<Entity> entities = GetEntities();

            RequestEngineClearPipeline();

            // inserting the system holder at the index 0 of the storage
            for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                storage[i].insert(storage[i].begin(), std::move(_components[i][_systemHolder]));
            }
            _systemHolder = 0;
            EngineReloadEditorMode();

            // moving the components from the storage to the ECS
            _components
                = std::move(storage);

            // calling all components to reload, this should update the pipeline
            for (int i = 0; i < cloneBase.size(); ++i) {
                std::visit([&](auto&& arg) {
                    arg.OnLoad();
                },
                    cloneBase[i]);
            }

            // we deleted the entities manually so let's stop the engine from doing it
            _skipFrame = true;
            _deletedThisFrame = std::queue<int>();

            // synchronizing the ids queue
            _usedIds = decltype(_usedIds)();
            _freeIds = decltype(_freeIds)();
            for (size_t i = 0; i < _components[0].size(); ++i) {
                _usedIds.push_back(i);
            }
            _editorEntityContext = _systemHolder;

            for (auto& cpt : cloneBase) {
                std::visit([&](auto&& arg) {
                    arg.OnLoad();
                },
                    cpt);
            }

            // calling OnAddComponent on all components
            for (Entity e : _usedIds) {
                ForEachComponent(e, [&](AnyCpt& cpt) {
                    std::visit([&](auto&& arg) {
                        arg.OnAddComponent(e);
                    },
                        cpt);
                });
            }

            NotifyEnginePipelineErased();

            // calling start on all components
            CallStart();
        }

        void EngineReloadEditorMode();

        /**
         * @brief Does the same as ReloadEntities, but does not delete the current
         * entities and return the result instead. Does not reload the .so files.
         *
         * @return AllCpt
         */
        AllCpt
        PrepareScene(const std::string& scenePath)
        {
            AllCpt result;
            for (auto& p : std::filesystem::directory_iterator(scenePath)) {
                if (p.path().filename() == std::to_string(_systemHolder))
                    continue;
                DumpEntity(p.path().string(), result);
            }
            return result;
        }

        void NotifyEnginePipelineErased();
        void RequestEngineClearPipeline();

        std::vector<Entity> GetEntities() const
        {
            return _usedIds;
        }

        /**
         * @brief Runs a loop an calls Update at each iteration.
         * The loop can be exited by calling Exit().
         * Custom functions can be ran before and after the Update call.
         *
         * @param preUpdateFunc
         * @param postUpdateFunc
         */
        void Run(
            std::function<void(void)> preUpdateFunc,
            std::function<void(void)> postUpdateFunc);

        Entity GetSystemHolder() const
        {
            return _systemHolder;
        }

        void Exit()
        {
            std::cout << green << "System exiting" << white << std::endl;
            _running = false;
        }

        /**
         * @brief Applies a function to all the components of the given entity.
         * The syntax to use is:
         * @code
         *
         * SYS.ForEachComponent(entity, [&](ecs::ECSImpl::AnyCpt &cpt) {
         *     std::visit([&](auto &&arg) {
         *        // do something with arg
         *     }, cpt);
         * });
         * @endcode
         * @param e
         * @param func
         */
        void ForEachComponent(Entity e, std::function<void(AnyCpt&)> func)
        {
            for (size_t i = 0; i < sizeof...(VanillaComponents); ++i) {
                AllCptOfSameType& cpt = _components[i];
                if (cpt.size() <= (unsigned int)e)
                    continue;
                for (size_t j = 0; j < cpt[e].size(); ++j) {
                    func(cpt[e][j]);
                }
            }
        }

        /**
         * @brief Calls a function on all the components of the given type. if the type is not found,
         * throws an exception.
         *
         * @tparam T
         * @param e
         * @param func
         */
        template <typename T>
        void ForEach(std::function<void(T&)> func)
        {
            size_t idx = 0;
            try {
                idx = _cptTypesIndexes.at(std::type_index(typeid(T)));
            } catch (std::exception& e) {
                throw std::logic_error("Component not found: " + std::string(typeid(T).name()));
            }
            AllCptOfSameType& cpt = _components[idx];
            for (auto e : _usedIds)
                for (auto& cpt : cpt[e]) {
                    func(std::get<T>(cpt));
                }
        }

        std::shared_ptr<graph::IGraphicalModule> GetGraphicalModule() { return _graphicalModule; }
        void SetGraphicalModule(std::shared_ptr<graph::IGraphicalModule> module) { _graphicalModule = module; }

        inline ResourceManager& GetResourceManager() { return _resourceManager; }
        InputManager& GetInputManager() { return _inputManager; }

        void SetEditorEntityContext(Entity e) { _editorEntityContext = e; }
        Entity GetEditorEntityContext() const { return _editorEntityContext; }
        int GetCptTypesNbr() const
        {
            return sizeof...(VanillaComponents);
        }
        std::vector<std::string> GetComponentsTypes()
        {
            std::vector<std::string> result;
            for (int i = 0; i < cloneBase.size(); ++i) {
                std::visit([&](auto&& arg) {
                    result.push_back(arg.GetClassName());
                },
                    cloneBase[i]);
            }
            return result;
        };

        bool FrameIsSkipped() const { return _skipFrame; }

        float GetDeltaTime() const;

        std::vector<std::string> GetVanillaList() {
            std::vector<std::string> result;

            for (auto i = 0; i < cloneBase.size(); ++i) {
                std::visit([&](auto &cpt) {
                    result.push_back(cpt.GetClassName());
                }, cloneBase[i]);
            }
            return result;
        }

    private:
        std::shared_ptr<graph::IGraphicalModule> _graphicalModule
            = nullptr;
        std::queue<int> _freeIds;
        std::vector<Entity> _usedIds;
        std::queue<int> _deletedThisFrame;

        Entity _systemHolder;

        AllCpt _components;
        std::unordered_map<std::type_index, size_t> _cptTypesIndexes;

        template <int... S>
        void initializeComponentIndexes(sequence<S...>)
        {
            ((_cptTypesIndexes[std::type_index(typeid(VanillaComponents))] = S), ...);
        }

        ResourceManager _resourceManager;
        InputManager _inputManager;

        bool _running = true;
        bool _skipFrame = true;
        std::vector<std::variant<VanillaComponents...>> cloneBase = { VanillaComponents()... };
        std::string m_savePath = "";
        float _deltaTime = 0.0f;

        Entity _editorEntityContext = 0;
    };

    template <typename... VanillaComponents>
    std::unique_ptr<ECS<VanillaComponents...>> ECS<VanillaComponents...>::System = nullptr;
    template <typename... VanillaComponents>
    typename ECS<VanillaComponents...>::WarningStream ECS<VanillaComponents...>::warn = WarningStream();
    template <typename... VanillaComponents>
    typename ECS<VanillaComponents...>::ErrorStream ECS<VanillaComponents...>::err = ErrorStream();
    template <typename... VanillaComponents>
    typename ECS<VanillaComponents...>::InfoStream ECS<VanillaComponents...>::info = InfoStream();
    template <typename... VanillaComponents>
    typename ECS<VanillaComponents...>::PromptStream ECS<VanillaComponents...>::prompt = PromptStream();
    template <typename... VanillaComponents>
    typename ECS<VanillaComponents...>::loglevel_t ECS<VanillaComponents...>::LogLevel = LOGINFO;
}
