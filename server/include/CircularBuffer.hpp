/*
** EPITECH PROJECT, 2023
** server
** File description:
** CircularBuffer.hpp
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
        bytes() = default;

        bytes(char* s, int size)
        {
            _data.insert(_data.end(), s, s + size);
        }

        bytes(std::string s)
        {
            _data.insert(_data.end(), s.begin(), s.end());
        }

        bytes(std::initializer_list<unsigned char> init_list)
        {
            _data.insert(_data.end(), init_list.begin(), init_list.end());
        }

        bytes(std::vector<unsigned char> v)
        {
            _data = v;
        }
        std::vector<unsigned char> _data;

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
            bytes result = *this;
            result += other;
            return result;
        }

        inline bytes operator+(const std::string& other) const
        {
            bytes result = *this;
            result += other;
            return result;
        }

        inline void resize(int size) { _data.resize(size); }

        inline std::vector<unsigned char>::iterator begin() { return _data.begin(); }

        inline std::vector<unsigned char>::iterator end() { return _data.end(); }

        inline std::vector<unsigned char>::const_iterator begin() const { return _data.begin(); }

        inline std::vector<unsigned char>::const_iterator end() const { return _data.end(); }

        inline std::vector<unsigned char>::iterator find(const bytes& delim)
        {
            return std::search(_data.begin(), _data.end(), delim.begin(), delim.end());
        }

        std::vector<unsigned char>::const_iterator find(const bytes& delim) const
        {
            return std::search(_data.begin(), _data.end(), delim.begin(), delim.end());
        }

        std::vector<unsigned char>::iterator find(const std::string& delim)
        {
            return std::search(_data.begin(), _data.end(), delim.begin(), delim.end());
        }

        std::vector<unsigned char>::const_iterator find(const std::string& delim) const
        {
            return std::search(_data.begin(), _data.end(), delim.begin(), delim.end());
        }

        std::vector<unsigned char>::iterator find(const char* delim)
        {
            return std::search(_data.begin(), _data.end(), delim, delim + strlen(delim));
        }

        std::vector<unsigned char>::const_iterator find(const char* delim) const
        {
            return std::search(_data.begin(), _data.end(), delim, delim + strlen(delim));
        }

        int compare(const bytes& other) const
        {
            for (int i = 0; i < _data.size(); ++i) {
                if (i >= other._data.size() || _data[i] != other._data[i]) {
                    std::cout << "nope because values are " << (int)_data[i] << " and " << (int)other._data[i] << std::endl;
                    return _data[i] - other._data[i];
                }
            }
            return 0;
        }

        int compare(const std::string& other) const
        {
            for (int i = 0; i < _data.size(); ++i) {
                if (i >= other.size() || _data[i] != other[i])
                    return _data[i] - other[i];
            }
            return 0;
        }

        int compare(std::size_t start, std::size_t n, const bytes& other) const
        {
            for (int i = start; i < start + n; ++i) {
                if (i >= _data.size() || i >= other._data.size() || _data[i] != other._data[i])
                    return _data[i] - other._data[i];
            }
            return 0;
        }

        std::ostream& operator<<(std::ostream& os) const
        {
            os << toString();
            return os;
        }

        std::string toString() const
        {
            std::stringstream ss;

            for (auto& byte : _data) {
                if (std::isalnum(byte))
                    ss << byte;
                else {
                    ss << " \\x"
                       << std::setfill('0') << std::setw(2)
                       << std::hex << static_cast<int>(byte)
                       << " ";
                }
            }
            return ss.str();
        }

        bool operator==(const bytes& other) const
        {
            for (int i = 0; i < _data.size(); ++i) {
                if (i >= other._data.size() || _data[i] != other._data[i])
                    return false;
            }
            return true;
        }

        bool operator==(const std::string& other) const
        {
            return compare(other) == 0;
        }

        bool operator!=(const bytes& other) const
        {
            return !(*this == other);
        }

        bool operator!=(const std::string& other) const
        {
            return !(*this == other);
        }

        template <typename T>
        static std::size_t find_last_of(T arr, std::function<bool(unsigned char)> f)
        {
            for (int i = arr.size() - 1; i >= 0; --i) {
                if (f(arr[i]))
                    return i;
            }
            return arr.size() - 1;
        }

        bool ends_with(const bytes& other) const
        {
            if (_data.size() < other._data.size())
                return false;

            return std::equal(_data.end() - other._data.size(), _data.end(), other._data.begin());
        }
    };

    /**
     * @brief This class implements a circular buffer. The user can Read the whole
     * buffer at once or stop at a given delimiter.
     *
     */
    class CircularBuffer {
    public:
        CircularBuffer(int size);
        ~CircularBuffer();

        /**
         * @brief Writes some _data in the buffer.
         *
         * @param _data
         */
        void Write(const bytes& _data);

        /**
         * @brief Returns the _data in the buffer until the given delimiter,
         * or the empty string if the delimiter is not found.
         *
         * @param delimiter
         * @return bytes
         */
        bytes ReadUntil(const bytes& delimiter);

        /**
         * @brief Returns the whole buffer.
         *
         * @return bytes
         */
        bytes Read();

        /**
         * @brief Returns the size of the buffer.
         *
         * @return int
         */
        int GetSize() const;

        /**
         * @brief Returns true if the buffer is empty, false otherwise.
         *
         * @return true
         * @return false
         */
        bool IsEmpty() const;

        /**
         * @brief Returns true if the buffer is full, false otherwise.
         *
         * @return true
         * @return false
         */
        bool IsFull() const;

    private:
        int _size;
        int _read;
        int _write;
        char* _buffer;
    };

}
