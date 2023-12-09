/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ToBytes.hpp
*/

#pragma once
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace bytes {

    class ToBytes {
    public:
        template <typename... T>
        void Load(const std::string& filepath, T&... args)
        {
            std::ifstream file(filepath);
            if (!file.is_open()) {
                throw std::runtime_error("File not found: " + filepath);
            }
            data.clear();
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            data.resize(size);
            file.read(reinterpret_cast<char*>(data.data()), size);
            file.close();
            it = data.begin();
            (Deserialize(data, it, args), ...);
        }

        template <typename... T>
        void Save(const std::string& filepath, T&... args)
        {
            data.clear();
            (Serialize(args, data), ...);

            std::ofstream file(filepath);
            if (!file.is_open()) {
                throw std::runtime_error("File not found: " + filepath);
            }
            file.write(reinterpret_cast<char*>(data.data()), data.size());
            file.close();
        }

        template <typename T>
        void Buffer(const T& object)
        {
            Serialize(object, data);
        }

        template <typename T>
        void ClearBuffer()
        {
            data.clear();
        }

        template <typename T>
        void GetBuffer(T& object)
        {
            it = data.begin();
            Deserialize(data, it, object);
        }

        std::vector<unsigned char> data;
        decltype(data)::iterator it;

        template <typename T, typename = void>
        struct is_iterable : std::false_type { };

        template <typename T>
        struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type { };

        template <typename T>
        static std::enable_if_t<!is_iterable<T>::value && !std::is_pointer<T>::value> Serialize(const T& object, std::vector<unsigned char>& data)
        {
            std::vector<unsigned char> tmp;
            tmp.resize(sizeof(T));
            memcpy(tmp.data(), &object, sizeof(T));
            data.insert(data.end(), tmp.begin(), tmp.end());
        }

        template <typename T>
        static std::enable_if_t<is_iterable<T>::value> Serialize(const T& object, std::vector<unsigned char>& data)
        {
            Serialize(object.size(), data);
            for (const auto& item : object) {
                Serialize(item, data);
            }
        }

        template <typename T>
        static std::enable_if_t<!is_iterable<T>::value && !std::is_pointer<T>::value> Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it, T& object)
        {
            memcpy(&object, &(*it), sizeof(T));
            it += sizeof(T);
        }

        template <typename T>
        static std::enable_if_t<is_iterable<T>::value> Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it, T& object)
        {
            size_t size;
            Deserialize(data, it, size);
            object.resize(size);
            for (size_t i = 0; i < size; ++i) {
                typename T::value_type item;
                Deserialize(data, it, item);
                object[i] = item;
            }
        }

        template <typename T>
        static std::enable_if_t<std::is_pointer<T>::value> Serialize(const T& object, std::vector<unsigned char>& data)
        {
            if (object == nullptr) {
                Serialize(false, data);
            } else {
                Serialize(true, data);
                Serialize(*object, data);
            }
        }

        template <typename T>
        static std::enable_if_t<std::is_pointer<T>::value> Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it, T& object)
        {
            bool isNotNull;
            Deserialize(data, it, isNotNull);
            if (isNotNull) {
                object = new typename std::remove_pointer<T>::type;
                Deserialize(data, it, *object);
            } else {
                object = nullptr;
            }
        }

        static void Serialize(const std::string& object, std::vector<unsigned char>& data)
        {
            Serialize(object.size(), data);
            for (int i = 0; i < object.size(); ++i) {
                Serialize(object[i], data);
            }
        }

        static void Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it, std::string& object)
        {
            size_t size;
            Deserialize(data, it, size);
            object.resize(size);
            for (size_t i = 0; i < size; ++i) {
                char item;
                Deserialize(data, it, item);
                object[i] = item;
            }
        }

        template <typename T>
        static void Serialize(std::shared_ptr<T> object, std::vector<unsigned char>& data)
        {
            if (object == nullptr) {
                Serialize(false, data);
            } else {
                Serialize(true, data);
                Serialize(*object, data);
            }
        }

        template <typename T>
        static void Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it, std::shared_ptr<T>& object)
        {
            bool isNotNull;
            Deserialize(data, it, isNotNull);
            if (isNotNull) {
                object = std::make_shared<T>();
                Deserialize(data, it, *object);
            } else {
                object = nullptr;
            }
        }

        template <typename T>
        static void Serialize(const std::unique_ptr<T>& object, std::vector<unsigned char>& data)
        {
            if (object == nullptr) {
                Serialize(false, data);
            } else {
                Serialize(true, data);
                Serialize(*object, data);
            }
        }

        template <typename T>
        static void Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it, std::unique_ptr<T>& object)
        {
            bool isNotNull;
            Deserialize(data, it, isNotNull);
            if (isNotNull) {
                object = std::make_unique<T>();
                Deserialize(data, it, *object);
            } else {
                object = nullptr;
            }
        }
    };
} // namespace bytes