/*
** EPITECH PROJECT, 2023
** cleanRType
** File description:
** GenericStorage.hpp
*/

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <queue>
#include <typeinfo>
#include <stdexcept>

namespace ecs {


    /**
     * @brief A Generic storage system. It is like a small ECS.
     * It also loosely implement the SParse Set pattern.
     *
     */
    class GenericStorageSystem {
        public:
            GenericStorageSystem() = default;
            ~GenericStorageSystem();

            /**
             * @brief Stores an object in the storage system. Returns the index
             * of the object in the storage system.
             *
             * @tparam T
             * @param component
             * @return size_t
             */
            template <typename T>
            size_t AddComponent(T *component) {
                if (_buckets.find(typeid(T).name()) == _buckets.end()) {
                    throw std::runtime_error("Component not registered: " + std::string(typeid(T).name()));
                }
                if (component == nullptr) {
                    throw std::runtime_error("Component cannot be null");
                }
                auto &bucket = _buckets[typeid(T).name()];
                size_t index;
                if (bucket.freeSlots.empty()) {
                    index = bucket.components.size();
                    bucket.components.push_back(component);
                    bucket.usedSlots.emplace_back(index);
                } else {
                    index = bucket.freeSlots.front();
                    bucket.freeSlots.pop();
                    bucket.components[index] = component;
                    bucket.usedSlots.emplace_back(index);
                }
                return index;
            };

            /**
             * @brief Removes an object from the storage system.
             *
             * @tparam T
             * @param index
             */
            template <typename T>
            void RemoveComponent(size_t index) {
                if (_buckets.find(typeid(T).name()) == _buckets.end()) {
                    throw std::runtime_error("Component not registered: " + std::string(typeid(T).name()));
                }
                auto &bucket = _buckets[typeid(T).name()];
                if (index >= bucket.components.size()) {
                    throw std::runtime_error("Index out of range: " + std::to_string(index));
                }
                bucket.destructor(bucket.components[index]);
                bucket.components[index] = nullptr;
                bucket.freeSlots.push(index);
                bucket.usedSlots.erase(std::remove(bucket.usedSlots.begin(), bucket.usedSlots.end(), index), bucket.usedSlots.end());
            };

            /**
             * @brief Returns the object at the given index.
             *
             * @tparam T
             * @param index
             * @return T*
             */
            template <typename T>
            T *GetComponent(size_t index) {
                if (_buckets.find(typeid(T).name()) == _buckets.end()) {
                    throw std::runtime_error("Component not registered: " + std::string(typeid(T).name()));
                }
                auto &bucket = _buckets[typeid(T).name()];
                if (index >= bucket.components.size() or not std::any_of(bucket.usedSlots.begin(), bucket.usedSlots.end(), [&](size_t i) { return i == index; })) {
                    throw std::runtime_error("Index points to an empty slot: " + std::to_string(index));
                }
                return static_cast<T *>(bucket.components[index]);
            };

            /**
             * @brief Registers a type in the storage system. This creates a destructor for the type.
             * It also gives the user a way to provide a system to run on the type.
             *
             * @tparam T
             * @param system
             */
            template <typename T>
            void RegisterComponent(std::function<void(T *)> sys) {
                if (_buckets.find(typeid(T).name()) != _buckets.end()) {
                    throw std::runtime_error("Component already registered: " + std::string(typeid(T).name()));
                }
                if (sys == nullptr) {
                    throw std::runtime_error("Sytem cannot be null");
                }
                static std::function<void(T *)> sysBackup = sys; // to prevent the system from being deleted when falling out of scope
                _buckets.emplace(typeid(T).name(), itemBucket{
                    std::vector<void *>(),
                    [](void *component) { delete static_cast<T *>(component); },
                    [&](void *component) {sysBackup(static_cast<T *>(component));},
                    std::queue<size_t>(),
                    std::vector<size_t>()
                });
            };

            /**
             * @brief Runs the system of the specified type. It will be applied on all the objects
             * of the type.
             *
             * @tparam T
             */
            template <typename T>
            void RunSystem() {
                if (_buckets.find(typeid(T).name()) == _buckets.end()) {
                    throw std::runtime_error("Component not registered: " + std::string(typeid(T).name()));
                }
                auto &bucket = _buckets[typeid(T).name()];
                for (auto i : bucket.usedSlots) {
                    if (bucket.components[i] != nullptr) {
                        bucket.system(static_cast<T *>(bucket.components[i]));
                    }
                }
            };

            template <typename T>
            void ForEach(std::function<void(T *)> func) {
                if (_buckets.find(typeid(T).name()) == _buckets.end()) {
                    throw std::runtime_error("Component not registered: " + std::string(typeid(T).name()));
                }
                auto &bucket = _buckets[typeid(T).name()];
                for (auto i : bucket.usedSlots) {
                    if (bucket.components[i] != nullptr)
                        func(static_cast<T *>(bucket.components[i]));
                }
            };

            template <typename T>
            void ForEach(std::function<void(T *, std::size_t)> func) {
                if (_buckets.find(typeid(T).name()) == _buckets.end()) {
                    throw std::runtime_error("Component not registered: " + std::string(typeid(T).name()));
                }
                auto &bucket = _buckets[typeid(T).name()];
                for (auto i : bucket.usedSlots) {
                    if (bucket.components[i] != nullptr)
                        func(static_cast<T *>(bucket.components[i]), i);
                }
            };


            /**
             * @brief maps a function call on several types of the generic storage system
             * using ForEach.
             *
             */
            template <auto Functor, typename... Types>
            void Map() {
                (ForEach<Types>(Functor), ...);
            };

            /**
             * @brief Runs the system of all specified types.
             *
             */
            template <typename... Types>
            void RunSystems() {
                (RunSystem<Types>(), ...);
            };

            /**
             * @brief Removes the objects at the given index from the
             * typeid string specified.
             *
             */
            void RemoveAt(size_t index, const std::string &type);

        private:
            typedef struct {
                std::vector<void *> components;
                std::function<void(void *)> destructor;
                std::function<void(void *)> system;
                std::queue<size_t> freeSlots;
                std::vector<size_t> usedSlots;
            } itemBucket;

            std::map<std::string, itemBucket> _buckets;
    };
}
