/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Coroutine.cpp
*/

#include "Coroutine.hpp"
#include <iostream>

namespace serv {

    Coroutine Coroutine::promise_type::get_return_object()
    {
        return Coroutine { std::coroutine_handle<promise_type>::from_promise(*this) };
    }

    std::suspend_never Coroutine::promise_type::initial_suspend()
    {
        return {};
    }

    std::suspend_always Coroutine::promise_type::final_suspend() noexcept
    {
        return {};
    }

    void Coroutine::promise_type::return_void()
    {
    }

    void Coroutine::promise_type::unhandled_exception()
    {
        std::cout << "Coroutine::promise_type::unhandled_exception" << std::endl;
    }

    Coroutine::Coroutine(Coroutine::handle_type handle)
        : _handle(handle)
    {
    }

    Coroutine::Coroutine(Coroutine&& other)
        : _handle(other._handle)
    {
        other._handle = nullptr;
    }

    Coroutine& Coroutine::operator=(Coroutine&& other)
    {
        if (_handle)
            _handle.destroy();
        _handle = other._handle;
        other._handle = nullptr;
        return *this;
    }

    Coroutine::~Coroutine()
    {
        if (_handle)
            _handle.destroy();
    }

    bool Coroutine::Resume()
    {
        if (!_handle)
            return false;
        if (_handle.done()) {
            _handle.destroy();
            _handle = nullptr;
            return false;
        } else {
            _handle.resume();
            return true;
        }
    }

    bool Coroutine::Done() const
    {
        return _handle.done();
    }

    ACoroutineExecutor::ACoroutineExecutor(std::function<Coroutine()> callback)
        : _routine(callback())
    {
    }

    bool ACoroutineExecutor::Execute()
    {
        if (!_routine.Done())
            _routine.Resume();
        return _routine.Done();
    }

}