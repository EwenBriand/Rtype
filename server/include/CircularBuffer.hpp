/*
** EPITECH PROJECT, 2023
** server
** File description:
** CircularBuffer.hpp
*/

#pragma once

#include "Bytes.hpp"
#include <algorithm>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace serv {

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
