#include <Helpers/Array.hpp>
#include <gtest/gtest.h>

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

// -------------------------------------------------------------------------- //

TEST(Array2D, initialize) {
    Array2D<int, 3, 2> arr = {{{0,1},{2,3},{4,5}}};
    EXPECT_EQ(arr[0][0], 0);
    EXPECT_EQ(arr[0][1], 1);
    EXPECT_EQ(arr[1][0], 2);
    EXPECT_EQ(arr[1][1], 3);
    EXPECT_EQ(arr[2][0], 4);
    EXPECT_EQ(arr[2][1], 5);
}