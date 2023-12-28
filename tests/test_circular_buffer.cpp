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

TEST(CircularBuffer, test2)
{
    serv::CircularBuffer buffer(17);
    buffer.Write(serv::bytes("123") + serv::SEPARATOR);
    auto data1 = buffer.ReadUntil(serv::SEPARATOR);
    ASSERT_EQ(data1.toString(), "123");
    buffer.Write(serv::bytes("45") + serv::SEPARATOR);
    auto data2 = buffer.ReadUntil(serv::SEPARATOR);
    ASSERT_EQ(data2.toString(), "45");
    buffer.Write(serv::bytes("6789") + serv::SEPARATOR);
    auto data3 = buffer.ReadUntil(serv::SEPARATOR);
    ASSERT_EQ(data3.toString(), "6789");
}