/*
** EPITECH PROJECT, 2023
** server
** File description:
** CircularBuffer.cpp
*/

#include "CircularBuffer.hpp"
#include <iostream>

namespace serv {

    CircularBuffer::CircularBuffer(int size)
        : _size(size)
        , _buffer(new char[size])
        , _read(0)
        , _write(0)
    {
    }

    CircularBuffer::~CircularBuffer()
    {
        delete[] _buffer;
    }

    void CircularBuffer::Write(const bytes& data)
    {
        for (int i = 0; i < data.size(); ++i) {
            _buffer[_write] = data[i];
            if (_write >= _size)
                _write = 0;
            else
                _write++;
        }
    }

    bytes CircularBuffer::ReadUntil(const bytes& delim)
    {
        bytes data;
        int i = 0;
        while (_read + i != _write) {
            data += _buffer[(_read + i) % _size];
            if (data.ends_with(delim)) {
                _read = (_read + i + 1) % _size;
                data.resize(data.size() - delim.size());
                return data;
            }
            i++;
        }
        return bytes();
    }

    bytes CircularBuffer::Read()
    {
        bytes data;
        while (_read != _write) {
            data += _buffer[_read];
            _read = (_read + 1) % _size;
        }
        return data;
    }

    bool CircularBuffer::IsEmpty() const
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
