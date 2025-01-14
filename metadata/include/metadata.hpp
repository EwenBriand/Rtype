/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** metadata.hpp
*/

#pragma once

#include "ToBytes.hpp"
#include <functional>
#include <map>
#include <sstream>
#include <string>

typedef std::function<void(const std::string&)> setter_t;
typedef std::function<void*(void)> getter_t;
typedef std::function<std::string(void)> tostring_t;

typedef struct metadata_s {
    std::string type;
    std::string name;
    size_t size;

    setter_t set;
    getter_t get;
    tostring_t tostring;
} metadata_t;

class AUserComponent;

typedef unsigned char uchar;

/**
 * @brief This macro is used to serialize a class or a function. It is used by
 * the parser of MetadataGenerator to generate the data associated with the
 * element serializeed.
 *
 */
#define serialize ;
#define needs(x) ;
#define dependsOn(x) ;
#define BUILD_MANUAL ;

#define GENERATE_METADATA(classname)        \
public:                                     \
    std::string GetClassName() const        \
    {                                       \
        return #classname;                  \
    }                                       \
    void Load(const std::string& filepath); \
    void Save(const std::string& filepath); \
    std::map<std::string, metadata_t> GetMetadata();

#define MANAGED_RESOURCE(classname)      \
    extern "C" {                         \
    AUserComponent* create_##classname() \
    {                                    \
        auto var = new classname();      \
        return var;                      \
    }                                    \
    }

#include <iostream>

template <typename T>
T fromString(const std::string& str);

template <typename T, typename = void>
struct is_iterable : std::false_type { };

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type { };

template <typename T>
std::enable_if_t<is_iterable<T>::value> fromStringImpl(const std::string& str, T& out, typename T::value_type* = 0)
{
    std::stringstream ss(str);
    T result;
    std::string token;
    while (std::getline(ss, token, ',')) {
        result.push_back(fromStringImpl<typename T::value_type>(token));
    }
    out = result;
}

template <typename T>
std::enable_if_t<!is_iterable<T>::value> fromStringImpl(const std::string& str, T& out)
{
    std::istringstream iss(str);
    T result;
    iss >> result;
    out = result;
}

template <>
inline std::string fromString(const std::string& str)
{
    return str;
}

template <typename T>
T fromString(const std::string& str)
{
    T result;
    fromStringImpl<T>(str, result);
    return result;
}

template <typename T>
std::string toStringImpl(const T& value);

template <typename T>
std::enable_if<bytes::ToBytes::is_iterable<T>::value, std::string> toStringImpl(const T& value)
{
    std::ostringstream oss;
    for (auto& v : value) {
        oss << toStringImpl<typename T::value_type>(v) << ",";
    }
    return oss.str();
}

template <typename T>
std::enable_if<!bytes::ToBytes::is_iterable<T>::value, std::string> toStringImpl(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <>
inline std::string toStringImpl<std::string>(const std::string& value)
{
    return value;
}

template <typename T>
std::string toString(const T& value)
{
    return toStringImpl<T>(value);
}

#define GENERATE_SETTER(name)                                  \
    "[&](const std::string &str) {"                            \
        + name + " = fromString<decltype(" + name + ")>(str);" \
                                                    "}"

#define GENERATE_GETTER(name) \
    "[&]() { return &" + name + "; }"

#define GENERATE_TO_STRING(name)     \
    "[&]() { "                       \
    "std::ostringstream oss;"        \
    "oss << "                        \
        + name + ";"                 \
                 "return oss.str();" \
                 "}"

// #define TEMPLATE_LOAD_VECTOR(name, contained_type)                            \
//     "\ttry {"                                                                 \
//     "auto &setting = config.lookup(\"cfg_"                                    \
//         + name + "\");\n"                                                     \
//                  "for (int i = 0; i < setting.getLength(); i++) {\n"          \
//         + contained_type + " value;\n"                                        \
//                            "setting.lookupValue(std::to_string(i), value);\n" \
//         + name + ".push_back(value);\n"                                       \
//                  "}\n"                                                        \
//                  "} catch (const libconfig::SettingNotFoundException &e) {"   \
//                  "std::cerr << \"Warning: could not find setting "            \
//         + name + " in file \" << filepath << std::endl;"                      \
//                  "}"

// #define TEMPLATE_SAVE_VECTOR(name, contained_type)                                                \
//     "auto &setting_" + name + " = root.add(\"cfg_" + name + "\", libconfig::Setting::TypeArray);" \
//                                                             "int "                                \
//         + name + "_i = 0;\n"                                                                      \
//                  "for (auto &value : "                                                            \
//         + name + ") {"                                                                            \
//                  "auto &setting = setting_"                                                       \
//         + name + ".add(std::to_string(" + name + "_i), libconfig::Setting::TypeFloat);"           \
//                                                  "setting = value;"                               \
//                                                  "}"

#define TEMPLATE_GENERATE_SETTER_VECTOR(name)                                                      \
    "[&](const std::string &str) {"                                                                \
    "std::stringstream "                                                                           \
        + name + "_ss(str);"                                                                       \
                 "std::string "                                                                    \
        + name + "_value;"                                                                         \
                 "while (getline("                                                                 \
        + name + "_ss, " + name + "_value, ',')) {"                                                \
        + name + ".push_back(fromString<decltype(" + name + ")::value_type>(" + name + "_value));" \
                                                                                       "}"         \
                                                                                       "}"

#define TEMPLATE_GENERATE_GETTER_VECTOR(name) \
    "[&]() { return &" + name + "; }"

#define TEMPLATE_GENERATE_TO_STRING_VECTOR(name) \
    "[&]() { "                                   \
    "std::ostringstream oss;"                    \
    "for (auto &value : "                        \
        + name + ") {"                           \
                 "oss << value << \",\";"        \
                 "}"                             \
                 "return oss.str();"             \
                 "}"

// #define TEMPLATE_LOAD_MAP(name)                                             \
//     "try {"                                                                 \
//     "auto &setting = config.lookup(\""                                      \
//         + name + "\");"                                                     \
//                  "for (int i = 0; i < setting.getLength(); i++) {"          \
//                  "auto &entry = setting[i];"                                \
//                  "std::string key = entry.getName();"                       \
//                  "decltype("                                                \
//         + name + "::mapped_type) value;"                                    \
//                  "entry.lookupValue(\"\", value);"                          \
//         + name + "[key] = value;"                                           \
//                  "}"                                                        \
//                  "} catch (const libconfig::SettingNotFoundException &e) {" \
//                  "std::cerr << \"Warning: could not find setting "          \
//         + name + " in file \" << filepath << std::endl;"                    \
//                  "}"

// #define TEMPLATE_SAVE_MAP(name)                                                               \
//     "auto &setting_" + name + " = root.add(\"" + name + "\", libconfig::Setting::TypeGroup);" \
//                                                         "for (auto &pair : "                  \
//         + name + ") {"                                                                        \
//                  "auto &setting = setting_"                                                   \
//         + name + ".add(pair.first, libconfig::Setting::TypeFloat);"                           \
//                  "setting = pair.second;"                                                     \
//                  "}"

#define TEMPLATE_GENERATE_SETTER_MAP(name)                                                         \
    "[&](const std::string &str) {"                                                                \
    "auto pairs = split(str, ',');"                                                                \
    "for (auto &pair : pairs) {"                                                                   \
    "auto key_value = split(pair, ':');"                                                           \
    "if (key_value.size() == 2) {"                                                                 \
        + name + "[key_value[0]] = fromString<decltype(" + name + "::mapped_type)>(key_value[1]);" \
                                                                  "}"                              \
                                                                  "}"                              \
                                                                  "}"

#define TEMPLATE_GENERATE_GETTER_MAP(name) \
    "[&]() { return &" + name + "; }"

#define TEMPLATE_GENERATE_TO_STRING_MAP(name)                          \
    "[&]() { "                                                         \
    "std::ostringstream oss;"                                          \
    "for (auto &pair : "                                               \
        + name + ") {"                                                 \
                 "oss << pair.first << \":\" << pair.second << \",\";" \
                 "}"                                                   \
                 "return oss.str();"                                   \
                 "}"
