#include <AH/Containers/BitArray.hpp>
#include <gtest/gtest.h>

USING_AH_NAMESPACE;

TEST(BitArray, construct) {
    BitArray<16> ba;
    for (int i = 0; i < 16; i++)
        EXPECT_EQ(ba.get(i), false);
}

TEST(BitArray, setClearGet) {
    BitArray<16> ba;
    ba.set(12);
    ba.set(7);
    for (int i = 0; i < 16; i++)
        if (i == 12 || i == 7)
            EXPECT_TRUE(ba.get(i));
        else
            EXPECT_FALSE(ba.get(i));
    ba.clear(12);
    for (int i = 0; i < 16; i++)
        if (i == 7)
            EXPECT_TRUE(ba.get(i));
        else
            EXPECT_FALSE(ba.get(i));
    ba.set(7, false);
    ba.set(6, true);
    for (int i = 0; i < 16; i++)
        if (i == 6)
            EXPECT_TRUE(ba.get(i));
        else
            EXPECT_FALSE(ba.get(i));
}

TEST(BitArray, outOfBounds) {
    BitArray<16> ba;
    EXPECT_THROW(ba.get(17), AH::ErrorException);
}
