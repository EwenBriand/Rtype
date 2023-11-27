/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CircularBuffer.cpp
*/

#include "Server.hpp"

namespace serv {

    CircularBuffer::CircularBuffer(int size) :
        _size(size), _buffer(new char[size]), _read(0), _write(0)
    {
    }

    CircularBuffer::~CircularBuffer() {
        delete[] _buffer;
    }

    void CircularBuffer::Write(const std::string &data) {
        for (int i = 0; i < data.size(); ++i) {
            _buffer[_write] = data[i];
            _write = (_write + 1) % _size;
        }
    }

    std::string CircularBuffer::ReadUntil(const std::string &delim)
    {
        std::string data;
        int i = 0;

        while (_read + i != _write) {
            data += _buffer[(_read + i) % _size];
            if (data.find(delim) != std::string::npos) {
                _read = (_read + i + 1) % _size;
                return data;
            }
            i++;
        }
        return "";
    }

    std::string CircularBuffer::Read() {
        std::string data;
        while (_read != _write) {
            data += _buffer[_read];
            _read = (_read + 1) % _size;
        }
        return data;
    }

    bool CircularBuffer::IsEmpty()  const
    {
        return _read == _write;
    }

    bool CircularBuffer::IsFull() const
    {
        return (_write + 1) % _size == _read;
    }

    int CircularBuffer::GetSize() const
    {
        return _size;
    }
}
