/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Coroutine.hpp
*/

#pragma once

#include <coroutine>
#include <functional>

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

        /**
         * @brief Resumes the coroutine's execution where it left off.
         * Returns true if the coroutine is done, false if it is not.
         *
         * @return true
         * @return false
         */
        bool Resume();

        /**
         * @brief Returns true if the coroutine is done, else false;
         *
         * @return true
         * @return false
         */
        bool Done() const;

    private:
        handle_type _handle;
    };

    /**
     * @brief This interface encapsulates a class dedicated to run a coroutine.
     * This is often useful when one whishes to associate more data with the
     * coroutine. Having a class wrapping a coroutine can make it easier to
     * communicate with the pseudo-asynchronous task it executes.
     *
     */
    class ICoroutineExecutor {
    public:
        virtual ~ICoroutineExecutor() = default;
        virtual bool Execute() = 0;
    };

    /**
     * @brief An abstract class deriving from the ICoroutineExecutor to provide
     * some common implementation for starting a coroutine.
     *
     */
    class ACoroutineExecutor : public ICoroutineExecutor {
    public:
        /**
         * @brief Construct a new ACoroutineExecutor object and starts its
         * execution from the callback provided.
         *
         * @param callback
         */
        ACoroutineExecutor(std::function<serv::Coroutine()> callback);

        /**
         * @brief Resumes the execution if the routine is not done.
         * Returns true if the routine is not done, false else.
         *
         */
        bool Execute() override;

    protected:
        serv::Coroutine _routine;
    };
} // namespace serv
