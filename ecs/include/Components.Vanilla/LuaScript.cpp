/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LuaScript.cpp
*/

#include "LuaScript.hpp"

LuaScript::LuaScript()
    : _luaRef(LUA.GetGlobal("dummy"))
{
}

void LuaScript::Update(int entityID)
{
    if (!_isLoaded)
        load();
    if (_luaRef.isTable()) {
        try {
            _luaRef["Update"](entityID);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void LuaScript::Start()
{
    if (!_isLoaded)
        load();
    if (_luaRef.isTable()) {
        try {
            _luaRef["Start"]();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void LuaScript::OnAddComponent(int entityID)
{
    if (!_isLoaded)
        load();
    if (_luaRef.isTable()) {
        try {
            _luaRef["OnAddComponent"](entityID);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void LuaScript::load()
{
    if (_scriptPath.empty())
        return;
    try {
        std::cout << "\rbefore load script" << std::endl;
        LUA.LoadScript(_scriptPath);
        std::cout << "\rafter load script" << std::endl;
        _isLoaded = true;
        _luaRef = getRef();
        std::cout << "\rafter get ref" << std::endl;
    } catch (const std::exception& e) {
        _isLoaded = false;
        std::cerr << e.what() << std::endl;
    }
}

luabridge::LuaRef LuaScript::getRef()
{
    if (!_isLoaded)
        throw std::runtime_error("LuaScript::getRef: script is not loaded");
    std::string className = _scriptPath.substr(_scriptPath.find_last_of('/') + 1);
    className = className.substr(0, className.find_last_of('.'));
    std::string uuid = "LuaScript_" + className + std::to_string((size_t)this);
    std::string command = uuid + " = " + className + ".new()";
    LUA.DoString(command);
    return LUA.GetGlobal(uuid);
}