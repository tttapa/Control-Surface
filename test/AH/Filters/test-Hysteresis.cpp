#include <gtest/gtest.h>

#include <AH/Filters/Hysteresis.hpp>
#include <algorithm>
#include <array>

using AH::Hysteresis;

TEST(Hysteresis, increasing) {
    using namespace std;
    constexpr size_t N = 18;
    Hysteresis<2> hyst;
    array<uint16_t, N> signal = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
    };
    array<bool, N> changedExpected = {
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
    };
    array<uint8_t, N> valuesExpected = {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
    };
    array<bool, N> changed;
    array<uint8_t, N> values;
    size_t i = 0;
    for (uint16_t s : signal) {
        changed[i] = hyst.update(s);
        values[i] = hyst.getValue();
        ++i;
    }
    EXPECT_EQ(changed, changedExpected);
    EXPECT_EQ(values, valuesExpected);
}

TEST(Hysteresis, decreasing) {
    using namespace std;
    constexpr size_t N = 18;
    Hysteresis<2> hyst;
    array<uint16_t, N> signal = {
        17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    };
    array<bool, N> changedExpected = {
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
    };
    array<uint8_t, N> valuesExpected = {
        4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
    };
    array<bool, N> changed;
    array<uint8_t, N> values;
    size_t i = 0;
    for (uint16_t s : signal) {
        changed[i] = hyst.update(s);
        values[i] = hyst.getValue();
        ++i;
    }
    EXPECT_EQ(changed, changedExpected);
    EXPECT_EQ(values, valuesExpected);
}

TEST(Hysteresis, updown) {
    using namespace std;
    constexpr size_t N = 20;
    Hysteresis<2> hyst;
    array<uint16_t, N> signal = {
        11, 12, 13,     //
        14, 13, 12, 11, //
        10, 9,  8,  7,  //
        6,  7,  8,  9,  //
        10, 11, 12, 13, //
        14,
    };
    array<bool, N> changedExpected = {
        1, 0, 0,    //
        1, 0, 0, 0, //
        1, 0, 0, 0, //
        1, 0, 0, 0, //
        1, 0, 0, 0, //
        1,
    };
    array<uint8_t, N> valuesExpected = {
        2, 2, 2,    //
        3, 3, 3, 3, //
        2, 2, 2, 2, //
        1, 1, 1, 1, //
        2, 2, 2, 2, //
        3,
    };
    array<bool, N> changed;
    array<uint8_t, N> values;
    size_t i = 0;
    for (uint16_t s : signal) {
        changed[i] = hyst.update(s);
        values[i] = hyst.getValue();
        ++i;
    }
    EXPECT_EQ(changed, changedExpected);
    EXPECT_EQ(values, valuesExpected);
}

TEST(Hysteresis, overflowIn) {
    using namespace std;
    constexpr size_t N = 10;
    Hysteresis<2, uint16_t, uint16_t> hyst;
    array<uint16_t, N> signal = {
        65535, 65530, 65533, 65534, 65535, 65534, 65533, 65532, 65531, 65530,
    };
    array<bool, N> changedExpected = {
        1, 1, 0, 1, 0, 0, 0, 0, 0, 1,
    };
    array<uint16_t, N> valuesExpected = {
        16383, 16382, 16382, 16383, 16383, 16383, 16383, 16383, 16383, 16382,
    };
    array<bool, N> changed;
    array<uint16_t, N> values;
    size_t i = 0;
    for (uint16_t s : signal) {
        changed[i] = hyst.update(s);
        values[i] = hyst.getValue();
        ++i;
    }
    EXPECT_EQ(changed, changedExpected);
    EXPECT_EQ(values, valuesExpected);
}

TEST(Hysteresis, overflowOut) {
    using namespace std;
    constexpr size_t N = 10;
    Hysteresis<2, uint16_t, uint8_t> hyst;
    array<uint16_t, N> signal = {
        1023, 1018, 1021, 1022, 1023, 1022, 1021, 1020, 1019, 1018,
    };
    array<bool, N> changedExpected = {
        1, 1, 0, 1, 0, 0, 0, 0, 0, 1,
    };
    array<uint8_t, N> valuesExpected = {
        255, 254, 254, 255, 255, 255, 255, 255, 255, 254,
    };
    array<bool, N> changed;
    array<uint8_t, N> values;
    size_t i = 0;
    for (uint16_t s : signal) {
        changed[i] = hyst.update(s);
        values[i] = hyst.getValue();
        ++i;
    }
    EXPECT_EQ(changed, changedExpected);
    EXPECT_EQ(values, valuesExpected);
}

TEST(Hysteresis, zeroBits) {
    using namespace std;
    constexpr size_t N = 10;
    Hysteresis<0> hyst;
    array<uint16_t, N> signal = {
        0, 1, 2, 0, 3, 2, 2, 3, 3, 0,
    };
    array<bool, N> changedExpected = {
        0, 1, 1, 1, 1, 1, 0, 1, 0, 1,
    };
    array<uint8_t, N> valuesExpected = {
        0, 1, 2, 0, 3, 2, 2, 3, 3, 0,
    };
    array<bool, N> changed;
    array<uint8_t, N> values;
    size_t i = 0;
    for (uint16_t s : signal) {
        changed[i] = hyst.update(s);
        values[i] = hyst.getValue();
        ++i;
    }
    EXPECT_EQ(changed, changedExpected);
    EXPECT_EQ(values, valuesExpected);
}
