/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LuaScript.hpp
*/

#pragma once
#include "Component.hpp"
#include "LuaModule.hpp"
#include "luaScriptingAPI.hpp"
#include "metadata.hpp"
#include <memory>
#include <string>

serialize class LuaScript : public ecs::Component<LuaScript> {
public:
    GENERATE_METADATA(LuaScript);
    LuaScript();
    void Update(int entityID);
    void Start();
    void OnAddComponent(int entityID);

private:
    void load();
    luabridge::LuaRef getRef();

    serialize std::string _scriptPath = "";
    luabridge::LuaRef _luaRef;
    bool _isLoaded = false;
};
