/*
** EPITECH PROJECT, 2023
** Visual Studio Live Share (Workspace)
** File description:
** LibUtils.hpp
*/

#pragma once

#include <string>

namespace lib {
    class LibUtils {
        public:
            static void *getLibHandle(const std::string &path);
            static void *getSymHandle(void *lib_handle, const std::string &sym_name);
            static void closeLibHandle(void *lib_handle);
    };
} // namespace lib
