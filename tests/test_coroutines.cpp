/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** test_coroutines.cpp
*/

#include "Coroutine.hpp"
#include <gtest/gtest.h>

TEST(Coroutine, test1)
{
    auto coroutine = serv::Coroutine([]() -> serv::Coroutine {
        std::cout << "Hello" << std::endl;
        co_await std::suspend_always {};
        std::cout << "World" << std::endl;
        co_await std::suspend_always {};
        std::cout << "!" << std::endl;
    }());

    int count = 1;
    while (!coroutine.Done()) {
        std::cout << "Resume " << count << std::endl;
        if (!coroutine.Resume())
            break;
        std::cout << "ouf!" << std::endl;
        count++;
    }
    ASSERT_EQ(count, 3);
}
