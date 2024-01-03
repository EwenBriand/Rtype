/*
** EPITECH PROJECT, 2023
** RTypeV0.0
** File description:
** Libutils.cpp
*/

#include "LibUtils.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
// #include <windows.h>
#define LIBHANDLE HMODULE
#define SYMHANDLE FARPROC
#define GETSYMBOL GetProcAddress
#define LOADLIB LoadLibrary
#define FREELIB FreeLibrary
#define ERRORLIB GetLastError
#else
#include <dlfcn.h>
#define LIBHANDLE void*
#define SYMHANDLE void*
#define GETSYMBOL dlsym
#define LOADLIB dlopen
#define FREELIB dlclose
#define ERRORLIB dlerror
#endif

namespace lib {

    void* LibUtils::getLibHandle(const std::string& path)
    {
        std::string path_copy = path;
        if (path.size() > 2 and path.substr(0, 2) == "./")
            path_copy = std::filesystem::current_path().string() + path.substr(1);
#ifdef _WIN32
        LIBHANDLE handle = LOADLIB(path_copy.c_str());
#else
        LIBHANDLE handle = LOADLIB(path_copy.c_str(), RTLD_LAZY);
#endif
        if (!handle) {
            std::cout << "execution path is : " << std::filesystem::current_path() << std::endl;
            std::cout << "nm result : " << std::endl;
            std::string nm_command = "nm " + path_copy + " | grep _ZTI5Laser";
            std::system(nm_command.c_str());

            throw std::runtime_error("Cannot load library: [" + path_copy + "] : " + std::string(ERRORLIB()));
        }
        return static_cast<void*>(handle);
    }

    void LibUtils::closeLibHandle(void* handle)
    {
        FREELIB(static_cast<LIBHANDLE>(handle));
    }

    void* LibUtils::getSymHandle(void* lib_handle, const std::string& sym_name)
    {
        SYMHANDLE handle = GETSYMBOL(static_cast<LIBHANDLE>(lib_handle), sym_name.c_str());

        if (!handle)
            throw std::runtime_error("Cannot load symbol : " + sym_name);
        return reinterpret_cast<void*>(handle);
    }

} // namespace lib
