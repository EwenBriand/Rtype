/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Observer.hpp
*/

#pragma once
#include "Timer.hpp"
#include <functional>
#include <vector>

namespace eng {
    struct byteImage {
        char* watchPtr;
        size_t size;

        std::vector<char> prevImage;

        byteImage(char* ptr, size_t size)
            : watchPtr(ptr)
            , size(size)
        {
            prevImage.reserve(size);
            for (int i = 0; i < size; ++i) {
                prevImage.push_back(watchPtr[i]);
            }
        }

        bool HasChanged()
        {
            for (int i = 0; i < size; ++i) {
                if (watchPtr[i] != prevImage[i]) {
                    return true;
                }
            }
            return false;
        }

        void Update()
        {
            for (int i = 0; i < size; ++i) {
                prevImage[i] = watchPtr[i];
            }
        }
    };

    class Observer {
    public:
        template <typename... T>
        Observer& RegisterTarget(std::function<void()> callback, T&... targets)
        {
            (_targets.emplace_back(reinterpret_cast<char*>(&targets), sizeof(targets)), ...);
            _callback = callback;
            return *this;
        }

        void Poll();

        void SetTimeout(float seconds);

    private:
        std::vector<byteImage> _targets;
        std::function<void()> _callback = nullptr;
        float _timeout;
        Timer _timer;
    };
} // namespace eng
