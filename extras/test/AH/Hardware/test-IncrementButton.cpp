#include <AH/Hardware/IncrementButton.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
USING_AH_NAMESPACE;

/**
 * Upon initialization, the button pin should be set to `INPUT_PULLUP`.
 */
TEST(IncrementButton, begin) {
    IncrementButton b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementButton, longPress) {
    testing::InSequence s;
    IncrementButton b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();

    // High initially
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1)
        .WillRepeatedly(Return(0));
    EXPECT_EQ(b.update(), IncrementButton::Nothing);

    // Fall → increment
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1)
        .WillRepeatedly(Return(1000));
    EXPECT_EQ(b.update(), IncrementButton::IncrementShort);

    // Stay low
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000 + LONG_PRESS_DELAY - 1));
    EXPECT_EQ(b.update(), IncrementButton::Nothing);

    // Long press → increment
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000 + LONG_PRESS_DELAY));
    EXPECT_EQ(b.update(), IncrementButton::IncrementLong);

    // Long press, still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(
            Return(1000 + LONG_PRESS_DELAY + LONG_PRESS_REPEAT_DELAY - 1));
    EXPECT_EQ(b.update(), IncrementButton::Nothing);

    // Long press, still pressed, repeat
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(
            Return(1000 + LONG_PRESS_DELAY + LONG_PRESS_REPEAT_DELAY));
    EXPECT_EQ(b.update(), IncrementButton::IncrementHold);

    // Release (long press)
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1)
        .WillRepeatedly(
            Return(1000 + LONG_PRESS_DELAY + LONG_PRESS_REPEAT_DELAY + 1000));
    EXPECT_EQ(b.update(), IncrementButton::ReleasedLong);

    // Fall → increment
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1)
        .WillRepeatedly(Return(10000));
    EXPECT_EQ(b.update(), IncrementButton::IncrementShort);

    // Stay low
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(10000 + LONG_PRESS_DELAY - 2));
    EXPECT_EQ(b.update(), IncrementButton::Nothing);

    // Release (short press)
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1)
        .WillRepeatedly(
            Return(10000 + LONG_PRESS_DELAY - 1));
    EXPECT_EQ(b.update(), IncrementButton::ReleasedShort);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}