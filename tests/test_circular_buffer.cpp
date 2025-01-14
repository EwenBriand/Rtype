/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** test_circular_buffer.cpp
*/

#include "CircularBuffer.hpp"
#include "ServerUdp.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(CircularBuffer, test1)
{
    serv::CircularBuffer buffer(50);
    buffer.Write(serv::bytes("Hello"));
    buffer.Write(serv::SEPARATOR);
    buffer.Write(serv::bytes("World"));
    buffer.Write(serv::SEPARATOR);

    serv::bytes data = buffer.ReadUntil(serv::SEPARATOR);
    ASSERT_EQ(data.toString(), "Hello");
    data = buffer.ReadUntil(serv::SEPARATOR);
    ASSERT_EQ(data.toString(), "World");
}

TEST(CircularBuffer, test3)
{
    serv::CircularBuffer buffer(17);
    buffer.Write(serv::bytes("10") + serv::SEPARATOR);
    buffer.Write(serv::bytes("11") + serv::SEPARATOR);
    auto data4 = buffer.ReadUntil(serv::SEPARATOR);
    ASSERT_EQ(data4.toString(), "");
}