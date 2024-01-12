/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Bytes.hpp
*/

#pragma once
#include <algorithm>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace serv {

    struct bytes {
        std::vector<unsigned char> _data;

        bytes() = default;

        bytes(char* s, int size);

        bytes(std::string s);

        bytes(std::initializer_list<unsigned char> init_list);

        bytes(std::vector<unsigned char> v);

        bytes(std::vector<int> v);

        inline std::vector<unsigned char> operator=(std::vector<unsigned char> v)
        {
            _data = v;
            return _data;
        }

        inline bool empty() const { return _data.empty(); }

        inline char* data() { return reinterpret_cast<char*>(_data.data()); }

        inline unsigned char operator[](int i) const { return _data[i]; }

        inline unsigned char& operator[](int i) { return _data[i]; }

        inline int size() const { return _data.size(); }

        inline void push_back(unsigned char c) { _data.push_back(c); }

        inline void insert(int i, unsigned char c) { _data.insert(_data.begin() + i, c); }

        inline void insert(int i, const bytes& other) { _data.insert(_data.begin() + i, other._data.begin(), other._data.end()); }

        inline void erase(int i) { _data.erase(_data.begin() + i); }

        inline void erase(std::size_t start, std::size_t end)
        {
            _data.erase(_data.begin() + start, _data.begin() + end);
        }

        inline bytes& operator=(const bytes& other)
        {
            _data = other._data;
            return *this;
        }

        inline bytes& operator=(const std::string& other)
        {
            _data.insert(_data.end(), other.begin(), other.end());
            return *this;
        }

        inline bytes& operator+=(const bytes& other)
        {
            _data.insert(_data.end(), other._data.begin(), other._data.end());
            return *this;
        }

        inline bytes& operator+=(const std::string& other)
        {
            _data.insert(_data.end(), other.begin(), other.end());
            return *this;
        }

        inline bytes& operator+=(const char* other)
        {
            _data.insert(_data.end(), other, other + strlen(other));
            return *this;
        }

        inline bytes& operator+=(const char other)
        {
            _data.push_back(other);
            return *this;
        }

        inline bytes& operator+=(const unsigned char other)
        {
            _data.push_back(other);
            return *this;
        }

        inline bytes operator+(const bytes& other) const
        {
            return bytes(*this) += other;
        }

        inline bytes operator+(const std::string& other) const
        {
            return bytes(*this) += other;
        }

        inline bytes operator=(const bytes& other) const
        {
            return other;
        }

        inline void resize(int size) { _data.resize(size); }

        inline std::vector<unsigned char>::iterator begin() { return _data.begin(); }

        inline std::vector<unsigned char>::iterator end() { return _data.end(); }

        inline std::vector<unsigned char>::const_iterator begin() const { return _data.begin(); }

        inline std::vector<unsigned char>::const_iterator end() const { return _data.end(); }

        template <typename T>
        static std::size_t find_last_of(T arr, std::function<bool(unsigned char)> f)
        {
            for (int i = arr.size() - 1; i >= 0; --i) {
                if (f(arr[i]))
                    return i;
            }
            return arr.size() - 1;
        }

        std::vector<unsigned char>::const_iterator find(const serv::bytes&);
        std::vector<unsigned char>::const_iterator find(const char*) const;
        std::vector<unsigned char>::iterator find(const char* delim);
        std::vector<unsigned char>::const_iterator find(const bytes& delim) const;
        std::vector<unsigned char>::iterator find(const std::string& delim);

        std::ostream&
        operator<<(std::ostream& os) const;
        std::string toString() const;
        bool operator==(const bytes& other) const;
        bool operator==(const std::string& other) const;
        bool operator!=(const bytes& other) const;
        bool operator!=(const std::string& other) const;
        bool ends_with(const bytes& other) const;
        int ToInt() const;

        template <typename... T>
        void Deserialize(T&... args)
        {
            size_t offset = 0;
            (Deserialize(args, offset), ...);
        }

        template <typename T>
        void Deserialize(T& object, size_t& offset)
        {
            std::memcpy(&object, _data.data() + offset, sizeof(T));
            offset += sizeof(T);
        }
    };
} // namespace serv
