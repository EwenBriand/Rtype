/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LuaScript.cpp
*/

#include "LuaScript.hpp"
#include "ECSImpl.hpp"
#include "LuaModule.hpp"

RefCell::RefCell(luabridge::LuaRef ref)
    : _ref(ref)
{
}

RefCell::RefCell(const RefCell& other)
    : _ref(other._ref)
{
}

RefCell::RefCell(RefCell&& other)
    : _ref(std::move(other._ref))
{
}

RefCell& RefCell::operator=(const RefCell& other)
{
    _ref = other._ref;
    return *this;
}

RefCell& RefCell::operator=(RefCell&& other)
{
    _ref = std::move(other._ref);
    return *this;
}

RefCell::~RefCell()
{
}

luabridge::LuaRef RefCell::getRef() const
{
    return _ref;
}

luabridge::LuaRef RefCell::operator[](const std::string& key) const
{
    return _ref[key];
}

luabridge::LuaRef RefCell::operator()() const
{
    return _ref;
}

LuaScript::LuaScript()
{
}

void LuaScript::Update(int entityID)
{
    if (!_isLoaded)
        load();
    if ((*_luaRef)().isTable()) {
        try {
            (*_luaRef)()["Update"](_luaRef, entityID);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void LuaScript::Start()
{
    if (!_isLoaded)
        load();
    if ((*_luaRef)().isTable()) {
        try {
            (*_luaRef)()["Start"](_luaRef);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void LuaScript::OnAddComponent(int entityID)
{
    if (!_isLoaded)
        load();
    if (_luaRef->getRef().isTable()) {
        try {
            _luaRef->getRef()["OnAddComponent"](_luaRef, entityID);
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
        LUA.LoadScript(_scriptPath);
        _isLoaded = true;
        _luaRef = std::make_shared<RefCell>(getRef());
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

void LuaScript::OnLoad()
{
    exportWrappers();
}

// ============================================================================================
// ================================ ECS BINDINGS EXPORT ========================================
// ============================================================================================

static CoreTransform& GetTransform(int entityID)
{
    return SYS.GetComponent<CoreTransform>(entityID);
}

void LuaScript::exportWrappers()
{
    if (_bindingsExported)
        return;
    _bindingsExported = true;

    luabridge::getGlobalNamespace(LUA.GetState())
        .addFunction("GetTransform", &GetTransform);
}