#include <AH/Math/Degrees.hpp>
#include <cmath>
#include <gtest/gtest.h>

using namespace AH;

TEST(Degrees, rad2deg) {
    ASSERT_FLOAT_EQ(rad2deg(M_PI), 180);
    ASSERT_FLOAT_EQ(rad2deg((float)M_PI), 180);
    ASSERT_FLOAT_EQ(rad2deg(2 * M_PI), 360);
    ASSERT_FLOAT_EQ(rad2deg(1.), 57.29577951308232);
}

TEST(Degrees, deg2rad) {
    ASSERT_FLOAT_EQ(deg2rad(180.), M_PI);
    ASSERT_FLOAT_EQ(deg2rad(360.), 2 * M_PI);
    ASSERT_FLOAT_EQ(deg2rad(57.29577951308232), 1);
}

TEST(Degrees, deg) {
    ASSERT_FLOAT_EQ(180_deg, M_PI);
    ASSERT_FLOAT_EQ(360_deg, 2 * M_PI);
    ASSERT_FLOAT_EQ(57.29577951308232_deg, 1);
}