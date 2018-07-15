#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Arduino.h>

using namespace ::testing;

TEST(Example, Example) {
    int i = 10;
    EXPECT_EQ(i, 10);
}

TEST(millis, millis) {
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillOnce(Return(1))
        .WillRepeatedly(Return(2));
    EXPECT_EQ(millis(), 1);
    EXPECT_EQ(millis(), 2);
}