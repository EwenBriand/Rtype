/*
** EPITECH PROJECT, 2023
** cleanRType
** File description:
** GenericStorage.cpp
*/

#include "GenericStorageSystem.hpp"

namespace ecs {
    GenericStorageSystem::~GenericStorageSystem() {
        for (auto &it : _buckets) {
            for (auto &component : it.second.components) {
                it.second.destructor(component);
            }
        }
    }

    void GenericStorageSystem::RemoveAt(std::size_t index, const std::string &type) {
        if (_buckets.find(type) == _buckets.end()) {
            throw std::runtime_error("Component not registered: " + type);
        }
        auto &bucket = _buckets[type];
        bucket.freeSlots.push(index);
        bucket.usedSlots.erase(std::remove(bucket.usedSlots.begin(), bucket.usedSlots.end(), index), bucket.usedSlots.end());
    }
}
