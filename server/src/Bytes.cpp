/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Bytes.cpp
*/

#include "Bytes.hpp"

namespace serv {
    bytes::bytes(char* s, int size)
    {
        _data.insert(_data.end(), s, s + size);
    }

    bytes::bytes(std::string s)
    {
        _data.insert(_data.end(), s.begin(), s.end());
    }

    bytes::bytes(std::initializer_list<unsigned char> init_list)
    {
        _data.insert(_data.end(), init_list.begin(), init_list.end());
    }

    bytes::bytes(std::vector<unsigned char> v)
    {
        _data = v;
    }

    bytes::bytes(std::vector<int> v)
    {
        _data.reserve(v.size() * sizeof(int));
        for (auto& i : v) {
            char* c = reinterpret_cast<char*>(&i);
            _data.insert(_data.end(), c, c + sizeof(int));
        }
    }

    std::vector<unsigned char>::const_iterator bytes::find(const bytes& delim) const
    {
        return std::search(_data.begin(), _data.end(), delim.begin(), delim.end());
    }

    std::vector<unsigned char>::iterator bytes::find(const std::string& delim)
    {
        return std::search(_data.begin(), _data.end(), delim.begin(), delim.end());
    }

    std::vector<unsigned char>::iterator bytes::find(const char* delim)
    {
        return std::search(_data.begin(), _data.end(), delim, delim + strlen(delim));
    }

    std::vector<unsigned char>::const_iterator bytes::find(const char* delim) const
    {
        return std::search(_data.begin(), _data.end(), delim, delim + strlen(delim));
    }

    std::ostream& bytes::operator<<(std::ostream& os) const
    {
        os << toString();
        return os;
    }

    std::string bytes::toString() const
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

    bool bytes::operator==(const bytes& other) const
    {
        for (int i = 0; i < _data.size(); ++i) {
            if (i >= other._data.size() || _data[i] != other._data[i])
                return false;
        }
        return true;
    }

    bool bytes::operator==(const std::string& other) const
    {
        for (int i = 0; i < _data.size(); ++i) {
            if (i >= other.size() || _data[i] != other[i])
                return false;
        }
        return true;
    }

    bool bytes::operator!=(const bytes& other) const
    {
        return !(*this == other);
    }

    bool bytes::operator!=(const std::string& other) const
    {
        return !(*this == other);
    }

    bool bytes::ends_with(const bytes& other) const
    {
        if (_data.size() < other._data.size())
            return false;

        return std::equal(_data.end() - other._data.size(), _data.end(), other._data.begin());
    }

    int bytes::ToInt() const
    {
        if (_data.size() < 4)
            throw std::runtime_error("bytes::ToInt: bytes size is less than 4");
        int result = 0;
        for (int i = 0; i < _data.size(); ++i) {
            result += _data[i] << (8 * i);
        }
        return result;
    }

} // namespace serv
