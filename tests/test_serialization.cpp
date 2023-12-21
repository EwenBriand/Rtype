#include "Animation.hpp"
#include "Animator.hpp"
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