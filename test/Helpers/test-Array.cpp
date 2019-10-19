#include <Def/Def.hpp>
#include <Helpers/ArrayHelpers.hpp>
#include <gtest-wrapper.h>

using namespace CS;

TEST(Array, initializeAndRetrieve) {
    Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, initializeAndRetrieveConst) {
    const Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, write) {
    Array<int, 6> arr = {42, 1, 2, 3, 4, 5};
    arr[0] = 0;
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, rangeFor) {
    Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    int i = 0;
    for (int &el : arr)
        EXPECT_EQ(el, i++);
}

TEST(Array, rangeForConst) {
    const Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    int i = 0;
    for (const int &el : arr)
        EXPECT_EQ(el, i++);
}

TEST(Array, equality) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    Array<int, 5> c = {1, 2, 3, 4, 6};
    EXPECT_EQ(a, a);
    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a != a);
    EXPECT_EQ(a, b);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_NE(a, c);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(Array, outOfBounds) {
    Array<int, 5> a = {};
    EXPECT_THROW(a[5], CS::ErrorException);
    const Array<int, 5> b = {};
    EXPECT_THROW(b[5], CS::ErrorException);
}

// -------------------------------------------------------------------------- //

TEST(Array2D, initialize) {
    Array2D<int, 3, 2> arr = {{{0, 1}, {2, 3}, {4, 5}}};
    EXPECT_EQ(arr[0][0], 0);
    EXPECT_EQ(arr[0][1], 1);
    EXPECT_EQ(arr[1][0], 2);
    EXPECT_EQ(arr[1][1], 3);
    EXPECT_EQ(arr[2][0], 4);
    EXPECT_EQ(arr[2][1], 5);
}

// -------------------------------------------------------------------------- //

TEST(generateIncrementalArray, simple) {
    auto x = generateIncrementalArray<unsigned int, 4>(2, 3);
    Array<unsigned int, 4> y = {2, 5, 8, 11};
    EXPECT_EQ(x, y);
}

TEST(generateArray, simple) {
    auto x = generateArray<unsigned int, 4>([i = 0u]() mutable { return i++; });
    Array<unsigned int, 4> y = {0, 1, 2, 3};
    EXPECT_EQ(x, y);
}

TEST(generateArray, simpleNoType) {
    auto x = generateArray<4>([i = 0u]() mutable { return i++; });
    Array<unsigned int, 4> y = {0, 1, 2, 3};
    EXPECT_EQ(x, y);
}

TEST(fillArray, simple) {
    struct S {
        int i;
        float f;
        bool operator!=(S o) const { return this->i != o.i || this->f != o.f; }
    };
    auto x = fillArray<S, 4>(2, 3.14f);
    Array<S, 4> y = {{{2, 3.14f}, {2, 3.14f}, {2, 3.14f}, {2, 3.14f}}};
    EXPECT_EQ(x, y);
}