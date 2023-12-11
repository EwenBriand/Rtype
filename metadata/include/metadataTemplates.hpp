/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** metadataTemplates.hpp
*/

#pragma once

#include <string>
#include <type_trait.hpp>



template<typename T, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type {};

template <typename T>
static std::enable_if_t<!is_iterable<T>::value> Serialize(const T &object, std::vector<unsigned char> &data)
{
    std::vector<unsigned char> tmp;
    tmp.resize(sizeof(T));
    memcpy(tmp.data(), &object, sizeof(T));
    data.insert(data.end(), tmp.begin(), tmp.end());
}

template <typename T>
static std::enable_if_t<is_iterable<T>::value> Serialize(const T &object, std::vector<unsigned char> &data)
{
    Serialize(object.size(), data);
    for (const auto &item : object)
    {
        Serialize(item, data);
    }
}

template <typename T>
static std::enable_if_t<!is_iterable<T>::value> Deserialize(const std::vector<unsigned char> &data, std::vector<unsigned char>::iterator &it, T &object)
{
    memcpy(&object, &(*it), sizeof(T));
    it += sizeof(T);
}

template <typename T>
static std::enable_if_t<is_iterable<T>::value> Deserialize(const std::vector<unsigned char> &data, std::vector<unsigned char>::iterator &it, T &object)
{
    size_t size;
    Deserialize(data, it, size);
    object.resize(size);
    for (size_t i = 0; i < size; ++i)
    {
        typename T::value_type item;
        Deserialize(data, it, item);
        object[i] = item;
    }
}


static const std::string THIS_DIR = "./";

static const std::string FOLDER_DOT_METADATA = ".metadata ";

static const std::string KW_CLASS = "class";
static const std::string KW_STRUCT = "struct";
static const std::string KW_SERIALIZE = "serialize";

static const std::string CLOSED_BRACKET = "}\n";

static const std::string FINALIZE_SAVE_FUNCTION = "\tconfig.writeFile(filepath.c_str());\n}" + CLOSED_BRACKET;
static const std::string FINALIZE_METADATA_FUNCTION = "\treturn metadata;\n" + CLOSED_BRACKET;

static const std::string METADATA_FILE_INCLUDES = R"(
/*
** This file was generated automatically by the metadata generator.
** Do not edit this file manually.
*/
#include <sstream>
#include <iostream>
#include "metadata.hpp"
)";

#define INIT_CMAKE(folder)                                                                         \
    "cmake_minimum_required(VERSION 3.0)\n"                                                        \
    "project(autogen)\n"                                                                           \
    "\n"                                                                                           \
    "set(CMAKE_CXX_STANDARD 20)\n"                                                                 \
    "include_directories("                                                                         \
        + folder + "\n"                                                                            \
        + folder + "../../../metadata/include/ ../../metadata/include/ \n"                         \
        + "../../ecs/include/\n"                                                                   \
        + " ../../ecs/raylib/raylib/include/ \n"                                                   \
        + folder + "/../../ecs/raylib/src/ \n"                                                     \
        + folder + "../../raylib/src/ \n"                                                          \
        + folder + "../../raylib/examples/shapes/ )\n\n"                                           \
        + "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY " + folder + ")\n"                                   \
        + "set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)\n" \
        + "link_directories(\n"                                                                    \
        + "\t${CMAKE_CURRENT_SOURCE_DIR}/ecs/raylib/raylib/\n"                                     \
        + "\t${CMAKE_CURRENT_SOURCE_DIR}/../ecs/raylib/raylib/\n"                                  \
        + ")\n"
