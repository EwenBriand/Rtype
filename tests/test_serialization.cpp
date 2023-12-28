#include "Animation.hpp"
#include "Animator.hpp"
#include "ServerUdp.hpp"
#include <ToBytes.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(serialization, most_types)
{
    int a = 1;
    int b = 2;
    int c = 3;

    std::vector<int*> vec;
    vec.push_back(&a);
    vec.push_back(&b);
    vec.push_back(&c);

    bytes::ToBytes bs;
    bs.Save("deleteme.bin", vec);

    std::vector<int*> vec2;
    bs.Load("deleteme.bin", vec2);

    for (int i = 0; i < vec2.size(); ++i) {
        ASSERT_EQ(*vec[i], *vec2[i]);
    }
    for (int i = 0; i < vec2.size(); ++i) {
        delete vec2[i];
    }
}

TEST(bytes, dataToInt)
{
    int a = 1;
    int b = 234;
    int c = 123456789;

    std::vector<int> vec = { a, b, c };
    serv::bytes bts(vec);

    ASSERT_EQ(bts.size(), 12);

    int a2 = 0;
    int b2 = 0;
    int c2 = 0;
    std::memcpy(&a2, bts._data.data(), sizeof(int));
    std::memcpy(&b2, bts._data.data() + sizeof(int), sizeof(int));
    std::memcpy(&c2, bts._data.data() + 2 * sizeof(int), sizeof(int));

    ASSERT_EQ(a, a2);
    ASSERT_EQ(b, b2);
    ASSERT_EQ(c, c2);
}