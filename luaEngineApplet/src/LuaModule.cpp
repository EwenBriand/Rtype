/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LuaModule.cpp
*/

#include "LuaModule.hpp"
#include "../include/luaExposed.hpp"

namespace lua {
    LuaModule::LuaModule()
    {
        _L = luaL_newstate();
        if (!_L)
            throw std::runtime_error("LuaModule::LuaModule: cannot initialize lua_State*");
        luaL_openlibs(_L);
        lua_init_exposed(_L);
        luaL_dostring(_L, R"(
            dummy = 1
            ComponentInterface = {
                Start = function(self) end,
                Update = function(self, entityID) end,
                Load = function(self, path) end,
                Save = function(self, path) end,
                OnAddComponent = function(self, entityID) end,
            }
        )");
    }

    LuaModule::~LuaModule()
    {
        // lua_close(_L);
    }

    LuaModule& LuaModule::GetInstance()
    {
        static LuaModule instance;
        return instance;
    }

    void LuaModule::LoadScript(const std::string& path)
    {
        if (!_L)
            throw std::runtime_error("LuaModule::LoadScript: lua_State* is null");
        if (luaL_dofile(_L, path.c_str()))
            throw std::runtime_error("LuaModule::LoadScript: " + std::string(lua_tostring(_L, -1)));
    }

    void LuaModule::DoString(const std::string& str)
    {
        if (!_L)
            throw std::runtime_error("LuaModule::DoString: lua_State* is null");
        if (luaL_dostring(_L, str.c_str()))
            throw std::runtime_error("LuaModule::DoString: " + std::string(lua_tostring(_L, -1)));
    }

    luabridge::LuaRef LuaModule::GetGlobal(const std::string& name)
    {
        if (!_L)
            throw std::runtime_error("LuaModule::GetGlobal: lua_State* is null");
        return luabridge::getGlobal(_L, name.c_str());
    }
}