/*
** EPITECH PROJECT, 2022
** ecs
** File description:
** ECS.hpp
*/

#pragma once

#include "Component.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "ECSImpl.hpp"
#include "Entity.hpp"
#include "IGraphicalModule.hpp"
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include <GenericStorageSystem.hpp>
#include <algorithm>
#include <any>
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

        template <typename T>
        struct isComponentType {
            static constexpr bool value = std::is_base_of<Component<T>, T>::value;
        }

        static ECS<VanillaComponents...>&
        GetSystem()
        {
            if (System == nullptr)
                System = std::make_unique<ECS<VanillaComponents...>>();
            return *(System.get());
        }

        static std::unique_ptr<ECS<VanillaComponents...>> System;

        ECS()
        {
            _systemHolder = RegisterEntity();
        }

    private:
        int _systemHolder;
    };
} // namespace ecs
