#include <gtest/gtest.h>

#include <MIDI_Senders/RelativeCCSender.hpp>

USING_CS_NAMESPACE;

TEST(RelativeCCSender, TWOS_COMPLEMENT) {
    RelativeCCSender::setMode(TWOS_COMPLEMENT);
    EXPECT_EQ(0b000'0000, RelativeCCSender::mapRelativeCC(0));
    EXPECT_EQ(0b000'0001, RelativeCCSender::mapRelativeCC(+1));
    EXPECT_EQ(0b011'1111, RelativeCCSender::mapRelativeCC(+63));
    EXPECT_EQ(0b100'0000, RelativeCCSender::mapRelativeCC(-64));
    EXPECT_EQ(0b100'0001, RelativeCCSender::mapRelativeCC(-63));
    EXPECT_EQ(0b111'1111, RelativeCCSender::mapRelativeCC(-1));
}

TEST(RelativeCCSender, BINARY_OFFSET) {
    RelativeCCSender::setMode(BINARY_OFFSET);
    EXPECT_EQ(0b000'0000, RelativeCCSender::mapRelativeCC(-64));
    EXPECT_EQ(0b000'0001, RelativeCCSender::mapRelativeCC(-63));
    EXPECT_EQ(0b011'1111, RelativeCCSender::mapRelativeCC(-1));
    EXPECT_EQ(0b100'0000, RelativeCCSender::mapRelativeCC(0));
    EXPECT_EQ(0b100'0001, RelativeCCSender::mapRelativeCC(1));
    EXPECT_EQ(0b111'1111, RelativeCCSender::mapRelativeCC(63));
}

TEST(RelativeCCSender, SIGN_MAGNITUDE) {
    RelativeCCSender::setMode(SIGN_MAGNITUDE);
    EXPECT_EQ(0b000'0000, RelativeCCSender::mapRelativeCC(0));
    EXPECT_EQ(0b000'0001, RelativeCCSender::mapRelativeCC(1));
    EXPECT_EQ(0b011'1111, RelativeCCSender::mapRelativeCC(63));
    // EXPECT_EQ(0b100'0000, RelativeCCSender::mapRelativeCC(0));
    EXPECT_EQ(0b100'0001, RelativeCCSender::mapRelativeCC(-1));
    EXPECT_EQ(0b111'1111, RelativeCCSender::mapRelativeCC(-63));
}