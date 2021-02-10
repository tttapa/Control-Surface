#include <Def/MIDIAddress.hpp>
#include <gtest/gtest.h>

using namespace CS;

TEST(MIDIAddress, init) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    EXPECT_EQ(m.getAddress(), 12);
    EXPECT_EQ(m.getChannel(), CHANNEL_10);
    EXPECT_EQ(m.getCableNumber(), CABLE_4);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDIAddress, invalid) {
    MIDIAddress m;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDIAddress, add) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    RelativeMIDIAddress n = {13, 3, 5};
    auto o = m + n;
    EXPECT_EQ(o.getAddress(), 12 + 13);
    EXPECT_EQ(o.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(o.getCableNumber(), CABLE_9);
    EXPECT_TRUE(o.isValid());
}

TEST(MIDIAddress, sub) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    RelativeMIDIAddress n = {11, 3, 2};
    auto o = m - n;
    EXPECT_EQ(o.getAddress(), 12 - 11);
    EXPECT_EQ(o.getChannel(), CHANNEL_10 - 3);
    EXPECT_EQ(o.getCableNumber(), CABLE_2);
    EXPECT_TRUE(o.isValid());
}

TEST(MIDIAddress, incrAdd) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    RelativeMIDIAddress n = {13, 3, 5};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 + 13);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(m.getCableNumber(), CABLE_9);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDIAddress, incrSub) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    RelativeMIDIAddress n = {11, 3, 2};
    m -= n;
    EXPECT_EQ(m.getAddress(), 12 - 11);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 - 3);
    EXPECT_EQ(m.getCableNumber(), CABLE_2);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDIAddress, addNegativeAddress) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    RelativeMIDIAddress n = {-1, 3, 5};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 - 1);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(m.getCableNumber(), CABLE_9);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDIAddress, addNegativeChannel) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    RelativeMIDIAddress n = {13, -3, 5};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 + 13);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 - 3);
    EXPECT_EQ(m.getCableNumber(), CABLE_9);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDIAddress, addNegativeCableNumber) {
    MIDIAddress m = {12, CHANNEL_10, CABLE_4};
    RelativeMIDIAddress n = {13, 3, -3};
    m += n;
    EXPECT_EQ(m.getAddress(), 12 + 13);
    EXPECT_EQ(m.getChannel(), CHANNEL_10 + 3);
    EXPECT_EQ(m.getCableNumber(), CABLE_1);
    EXPECT_TRUE(m.isValid());
}

TEST(MIDIAddress, addInvalidLHS) {
    MIDIAddress m;
    RelativeMIDIAddress n = {13, 3, 4};
    m += n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDIAddress, addInvalidRHS) {
    MIDIAddress m = {13, CHANNEL_3, CABLE_5};
    RelativeMIDIAddress n;
    m += n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDIAddress, subInvalidLHS) {
    MIDIAddress m;
    RelativeMIDIAddress n = {13, 3, 4};
    m -= n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDIAddress, subInvalidRHS) {
    MIDIAddress m = {13, CHANNEL_3, CABLE_5};
    RelativeMIDIAddress n;
    m -= n;
    EXPECT_FALSE(m.isValid());
}

TEST(MIDIAddress, equals) {
    {
        MIDIAddress m = {12, CHANNEL_10, CABLE_4};
        MIDIAddress n = {12, CHANNEL_10, CABLE_4};
        EXPECT_EQ(m, n);
        EXPECT_TRUE(m == n);
        EXPECT_FALSE(m != n);
    }
    {
        MIDIAddress m = {13, CHANNEL_10, CABLE_4};
        MIDIAddress n = {12, CHANNEL_10, CABLE_4};
        EXPECT_NE(m, n);
        EXPECT_TRUE(m != n);
        EXPECT_FALSE(m == n);
    }
    {
        MIDIAddress m = {12, CHANNEL_11, CABLE_4};
        MIDIAddress n = {12, CHANNEL_10, CABLE_4};
        EXPECT_NE(m, n);
        EXPECT_TRUE(m != n);
        EXPECT_FALSE(m == n);
    }
    {
        MIDIAddress m = {12, CHANNEL_10, CABLE_4};
        MIDIAddress n = {12, CHANNEL_10, CABLE_5};
        EXPECT_NE(m, n);
        EXPECT_TRUE(m != n);
        EXPECT_FALSE(m == n);
    }
    {
        MIDIAddress m;
        MIDIAddress n = {12, CHANNEL_10, CABLE_5};
        EXPECT_FALSE(m != n);
        EXPECT_FALSE(m == n);
    }
}