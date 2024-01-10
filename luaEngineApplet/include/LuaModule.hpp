/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** LuaModule.hpp
*/

#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

namespace lua {
    /**
     * @brief Lets the user easily call functions from a lua script. Provides utility functions to expose c++ functions to lua.
     *
     */
    class LuaModule {
    public:
        LuaModule();
        ~LuaModule();

        /**
         * @brief returns the instance of the singleton
         *
         * @return LuaModule&
         */
        static LuaModule& GetInstance();

        /**
         * @brief Loads a lua script
         *
         * @param path
         */
        void LoadScript(const std::string& path);

        void DoString(const std::string& str);

        luabridge::LuaRef GetGlobal(const std::string& name);

        template <typename C, typename R, typename... Args>
        R Call(const std::string& funcName, C* obj, Args... args)
        {
            if (!_L)
                throw std::runtime_error("LuaModule::Call: lua_State* is null");
            try {
                luabridge::LuaRef func = luabridge::getGlobal(_L, funcName.c_str());
                if (func.isFunction())
                    return func(obj, args...);
                else
                    throw std::runtime_error("LuaModule::Call: " + funcName + " is not a function");
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                return R();
            }
        }

        template <typename R, typename... Args>
        R Call(const std::string& funcName, Args... args)
        {
            if (!_L)
                throw std::runtime_error("LuaModule::Call: lua_State* is null");
            try {
                luabridge::LuaRef func = luabridge::getGlobal(_L, funcName.c_str());
                if (func.isFunction())
                    return func(args...);
                else
                    throw std::runtime_error("LuaModule::Call: " + funcName + " is not a function");
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                return R();
            }
        }

        template <typename C, typename R>
        R Call(const std::string& funcName, C* obj)
        {
            if (!_L)
                throw std::runtime_error("LuaModule::Call: lua_State* is null");
            try {
                luabridge::LuaRef func = luabridge::getGlobal(_L, funcName.c_str());
                if (func.isFunction())
                    return func(obj);
                else
                    throw std::runtime_error("LuaModule::Call: " + funcName + " is not a function");
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                return R();
            }
        }

        template <typename C, typename... Args>
        void Call(const std::string& funcName, C* obj, Args... args)
        {
            if (!_L)
                throw std::runtime_error("LuaModule::Call: lua_State* is null");
            try {
                luabridge::LuaRef func = luabridge::getGlobal(_L, funcName.c_str());
                if (func.isFunction())
                    func(obj, args...);
                else
                    throw std::runtime_error("LuaModule::Call: " + funcName + " is not a function");
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }

        template <typename... Args>
        void Call(const std::string& funcName, Args... args)
        {
            if (!_L)
                throw std::runtime_error("LuaModule::Call: lua_State* is null");
            try {
                luabridge::LuaRef func = luabridge::getGlobal(_L, funcName.c_str());
                if (func.isFunction())
                    func(args...);
                else
                    throw std::runtime_error("LuaModule::Call: " + funcName + " is not a function");
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }

        inline lua_State* GetState() { return _L; }

    private:
        lua_State* _L;
    };
}

#define LUA lua::LuaModule::GetInstance()
