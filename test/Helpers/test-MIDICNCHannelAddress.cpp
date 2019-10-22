#include <Def/MIDICNChannelAddress.hpp>
#include <gtest-wrapper.h>

using namespace CS;

TEST(MIDICNChannelAddress, init) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    EXPECT_EQ(m.getAddress(), 12);
    EXPECT_EQ(m.getChannel(), CHANNEL_10);
    EXPECT_EQ(m.getCableNumber(), 4);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDICNChannelAddress, invalid) {
    MIDICNChannelAddress m;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDICNChannelAddress, add) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    RelativeMIDICNChannelAddress n = {13, 3, 5};
    auto o = m + n;
    EXPECT_EQ(o.getAddress(), 12 + 13);
    EXPECT_EQ(o.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(o.getCableNumber(), 4 + 5);
    EXPECT_TRUE(o.isValid());
}

TEST(MIDICNChannelAddress, sub) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    RelativeMIDICNChannelAddress n = {11, 3, 2};
    auto o = m - n;
    EXPECT_EQ(o.getAddress(), 12 - 11);
    EXPECT_EQ(o.getChannel(), CHANNEL_10 - 3);
    EXPECT_EQ(o.getCableNumber(), 4 - 2);
    EXPECT_TRUE(o.isValid());
}

TEST(MIDICNChannelAddress, incrAdd) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    RelativeMIDICNChannelAddress n = {13, 3, 5};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 + 13);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(m.getCableNumber(), 4 + 5);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDICNChannelAddress, incrSub) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    RelativeMIDICNChannelAddress n = {11, 3, 2};
    m -= n;
    EXPECT_EQ(m.getAddress(), 12 - 11);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 - 3);
    EXPECT_EQ(m.getCableNumber(), 4 - 2);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDICNChannelAddress, addNegativeAddress) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    RelativeMIDICNChannelAddress n = {-1, 3, 5};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 - 1);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(m.getCableNumber(), 4 + 5);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDICNChannelAddress, addNegativeChannel) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    RelativeMIDICNChannelAddress n = {13, -3, 5};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 + 13);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 - 3);
    EXPECT_EQ(m.getCableNumber(), 4 + 5);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDICNChannelAddress, addNegativeCableNumber) {
    MIDICNChannelAddress m = {12, CHANNEL_10, 4};
    RelativeMIDICNChannelAddress n = {13, 3, -3};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 + 13);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(m.getCableNumber(), 4 - 3);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDICNChannelAddress, addInvalidLHS) {
    MIDICNChannelAddress m;
    RelativeMIDICNChannelAddress n = {13, 3, 4};
    m += n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDICNChannelAddress, addInvalidRHS) {
    MIDICNChannelAddress m = {13, CHANNEL_3, 5};
    RelativeMIDICNChannelAddress n;
    m += n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDICNChannelAddress, subInvalidLHS) {
    MIDICNChannelAddress m;
    RelativeMIDICNChannelAddress n = {13, 3, 4};
    m -= n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDICNChannelAddress, subInvalidRHS) {
    MIDICNChannelAddress m = {13, CHANNEL_3, 5};
    RelativeMIDICNChannelAddress n;
    m -= n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDICNChannelAddress, equals) {
    {
        MIDICNChannelAddress m = {12, CHANNEL_10, 4};
        MIDICNChannelAddress n = {12, CHANNEL_10, 4};
        EXPECT_EQ(m, n);
        EXPECT_TRUE(m == n);
        EXPECT_FALSE(m != n);
    }
    {
        MIDICNChannelAddress m = {13, CHANNEL_10, 4};
        MIDICNChannelAddress n = {12, CHANNEL_10, 4};
        EXPECT_NE(m, n);
        EXPECT_TRUE(m != n);
        EXPECT_FALSE(m == n);
    }
    {
        MIDICNChannelAddress m = {12, CHANNEL_11, 4};
        MIDICNChannelAddress n = {12, CHANNEL_10, 4};
        EXPECT_NE(m, n);
        EXPECT_TRUE(m != n);
        EXPECT_FALSE(m == n);
    }
    {
        MIDICNChannelAddress m = {12, CHANNEL_10, 4};
        MIDICNChannelAddress n = {12, CHANNEL_10, 5};
        EXPECT_NE(m, n);
        EXPECT_TRUE(m != n);
        EXPECT_FALSE(m == n);
    }
    {
        MIDICNChannelAddress m;
        MIDICNChannelAddress n = {12, CHANNEL_10, 5};
        EXPECT_FALSE(m != n);
        EXPECT_FALSE(m == n);
    }
}