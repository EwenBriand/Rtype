/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Coroutine.hpp
*/

#pragma once

#include <coroutine>

namespace serv {
    class Coroutine {
    public:
        struct promise_type {
            Coroutine get_return_object();
            std::suspend_never initial_suspend();
            std::suspend_always final_suspend() noexcept;
            void return_void();
            void unhandled_exception();
        };

        using handle_type = std::coroutine_handle<promise_type>;

        Coroutine(handle_type handle);
        Coroutine(Coroutine&& other);
        Coroutine& operator=(Coroutine&& other);
        Coroutine(const Coroutine&) = delete;
        Coroutine& operator=(const Coroutine&) = delete;
        ~Coroutine();

        bool Resume();
        bool Done() const;

    private:
        handle_type _handle;
    };
} // namespace serv
