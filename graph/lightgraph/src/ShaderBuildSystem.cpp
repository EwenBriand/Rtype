/*
** EPITECH PROJECT, 2023
** graph
** File description:
** ShaderBuildSystem.cpp
*/

#include "ShaderBuildSystem.hpp"

namespace lg {
    std::shared_ptr<ShaderBuildSystem> ShaderBuildSystem::Singleton = nullptr;
    ShaderBuildSystem::ShaderBuildSystem()
    {
        if (Singleton != nullptr) {
            throw std::runtime_error("ShaderBuildSystem is a singleton, cannot create more than one instance. Consider using ShaderBuildSystem.Get() instead.");
        }
    }

    ShaderBuildSystem &ShaderBuildSystem::Get()
    {
        if (Singleton == nullptr)
            Singleton = std::make_shared<ShaderBuildSystem>();
        return *Singleton;
    }

    ShaderBuilder &ShaderBuildSystem::operator[](const std::string &name)
    {
        if (_builders.find(name) == _builders.end())
            _builders[name] = ShaderBuilder();
        return _builders[name];
    }

    void ShaderBuildSystem::BuildAll()
    {
        int completion = 0;
        int total = _builders.size();

        for (auto &builder : _builders) {
            std::cout << "Building shaders programs... [" << completion + 1 << "/" << total << "]\n";
            builder.second.Build();
        }
    }
}
