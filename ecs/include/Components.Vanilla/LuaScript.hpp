/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LuaScript.hpp
*/

#pragma once
#include "Component.hpp"
#include "luaScriptingAPI.hpp"
#include "metadata.hpp"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>
#include <memory>
#include <string>

class RefCell {
public:
    RefCell(luabridge::LuaRef ref);
    RefCell(const RefCell& other);
    RefCell(RefCell&& other);
    RefCell& operator=(const RefCell& other);
    RefCell& operator=(RefCell&& other);
    ~RefCell();
    luabridge::LuaRef getRef() const;

    luabridge::LuaRef operator()() const;
    luabridge::LuaRef operator[](const std::string&) const;

private:
    luabridge::LuaRef _ref;
};

serialize class LuaScript : public ecs::Component<LuaScript> {
public:
    GENERATE_METADATA(LuaScript);
    LuaScript();
    void Update(int entityID);
    void Start();
    void OnAddComponent(int entityID);
    void OnLoad();

private:
    void load();

    /**
     * @brief Called once when the component is loaded, exports ECS bindings to lua
     *
     */
    void exportWrappers();
    luabridge::LuaRef getRef();

    serialize std::string _scriptPath = "";
    std::shared_ptr<RefCell> _luaRef = nullptr;
    bool _isLoaded = false;
    bool _bindingsExported = false;
};
