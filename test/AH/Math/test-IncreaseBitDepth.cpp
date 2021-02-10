#include <gtest/gtest.h>

#include <AH/Math/IncreaseBitDepth.hpp>
#include <algorithm>
#include <array>

USING_AH_NAMESPACE;

TEST(IncreaseBitDepth, IncreaseBitDepth) {
    std::array<uint8_t, 8> in = {
        0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111,
    };
    std::array<uint8_t, 8> out;
    std::transform(std::begin(in), std::end(in), std::begin(out),
                   [](uint8_t in) {
                       return increaseBitDepth<7, 3, uint8_t, uint8_t>(in);
                   });
    std::array<uint8_t, 8> expected = {
        0b000'0000, 0b001'0010, 0b010'0100, 0b011'0110,
        0b100'1001, 0b101'1011, 0b110'1101, 0b111'1111,
    };
    EXPECT_EQ(out, expected);
}
