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