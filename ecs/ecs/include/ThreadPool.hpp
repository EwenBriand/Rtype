// /*
// ** EPITECH PROJECT, 2023
// ** metadata
// ** File description:
// ** ThreadPool.hpp
// */

#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <future>
#include <mutex>
#include <condition_variable>
#include "./Types.hpp"

namespace td {
    /**
     * @brief A thread pool that can be used to run tasks in parallel.
     * This class is templated by the number of threads it can run.
     * If the number of threads is reached, the task is not added and will be ignored.
     *
     * If the program is terminated while some tasks are still running, the behaviour is
     * undefined and the program may crash if the user does not let it exit gracefully.
     * As such, the use of Ctrl-C is not recommended unless the user
     * implements the necessary logic to interrupt the tasks upon the SIGINT signal.
     *
     * @tparam threadCount
     */
    template <unsigned int threadCount>
    class ThreadPool {
        public:
            ThreadPool() = default;
            ~ThreadPool() {
                for (auto &task : m_tasks) {
                    task->wait();
                }
            }

            /**
             * @brief Runs a task asynchronously.
             * If the thread pool is full, the task is not added and is lost.
             *
             * @tparam Func
             * @tparam Args
             * @param func
             * @param args
             */
            template<typename Func, typename... Args>
            void AddTask(Func func, Args... args ) {
                ClearTerminatedTasks();
                if (m_tasks.size() >= threadCount) {
                    return;
                }
                m_tasks.emplace_back(
                    std::make_shared<std::future<void>>(
                        std::async(std::launch::async, func, args...)
                    )
                );
                ++m_threadCount;
            }

            void ClearTerminatedTasks() {
                decltype(m_tasks) cleanedTasks;
                for (auto &task : m_tasks) {
                    if (task->wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
                        cleanedTasks.push_back(task);
                    } else {
                        --m_threadCount;
                    }
                }
                m_tasks = std::move(cleanedTasks);
            }
        private:
            std::vector<std::shared_ptr<std::future<void>>> m_tasks;
            unsigned int m_threadCount = 0;
    };

    template <unsigned int threadCount>
    class ThreadedQueue {
        public:
            ThreadedQueue() :
                m_mutex(std::make_shared<std::mutex>()),
                m_cv(std::make_shared<std::condition_variable>())
            {
                Run();
            };

            ~ThreadedQueue() {
                m_thread.join();
            };

            template<typename Func, typename... Args>
            void Push(Func func, Args... args ) {
                std::lock_guard<std::mutex> lock(*m_mutex);
                if (m_queue.size() >= threadCount) {
                    m_cv->notify_one();
                    return;
                }
                m_queue.emplace([func, args...]() {
                    func(args...);
                });
                m_cv->notify_one();
            }

            void Run() {
                m_thread = std::thread([&]() {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(*m_mutex);
                            m_cv->wait(lock, [&]() {
                                return !m_queue.empty();
                            });
                            task = m_queue.front();
                            m_queue.pop();
                        }
                        task();
                    }
                });
            }
        private:
            std::thread m_thread;
            std::queue<std::function<void()>> m_queue;
            std::shared_ptr<std::mutex> m_mutex;
            std::shared_ptr<std::condition_variable> m_cv;
    };
}
