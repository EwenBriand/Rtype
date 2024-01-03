/*
** EPITECH PROJECT, 2023
** server
** File description:
** ThreadSafeQueue.hpp
*/

#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>

namespace serv {
    /**
     * @brief This class implements a thread safe queue.
     *
     * @tparam T
     */
    template <typename T>
    class ThreadSafeQueue {
    public:
        ThreadSafeQueue() = default;
        ~ThreadSafeQueue() = default;

        /**
         * @brief Pushes an element in the queue.
         *
         * @param element
         */
        void Push(const T& element)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(std::move(element));
        }

        /**
         * @brief Pops an element from the queue.
         *
         * @return T
         */
        T Pop()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty())
                throw std::runtime_error("Queue is empty");
            T element = std::move(_queue.front());
            _queue.pop();
            return element;
        }

        /**
         * @brief Returns the size of the queue.
         *
         * @return int
         */
        int GetSize() const
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }

        /**
         * @brief Returns true if the queue is empty, false otherwise.
         *
         * @return true
         * @return false
         */
        bool IsEmpty() const
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.empty();
        }

        std::size_t Size() const
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }

        void Resize(std::size_t size)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.resize(size);
        }

    private:
        std::queue<T> _queue;
        mutable std::mutex _mutex;
    };
}
