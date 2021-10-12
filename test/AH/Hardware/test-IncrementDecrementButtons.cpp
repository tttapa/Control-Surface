#include <AH/Hardware/IncrementDecrementButtons.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
USING_AH_NAMESPACE;

/**
 * Upon initialization, the button pin should be set to `INPUT_PULLUP`.
 */
TEST(IncrementDecrementButtons, begin) {
    IncrementDecrementButtons b({2, 3});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    b.begin();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementDecrementButtons, testAllCombinations) {
    IncrementDecrementButtons b({2, 3});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    b.begin();

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    unsigned long time = 0;

    // High initially
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Fall 1 → increment
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::IncrementShort);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());
    
    time += LONG_PRESS_DELAY - 1;

    // Low 1
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1;

    // Low 1 long press
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::IncrementLong);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += LONG_PRESS_REPEAT_DELAY - 1;

    // Low 1
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1;

    // Low 1 long press repeat
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::IncrementHold);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    // Fall 2 → reset
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Reset);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Both rise
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Both released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Fall 2 → Decrement
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::DecrementShort);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());
    
    time += LONG_PRESS_DELAY - 1;

    // Low 2
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1;

    // Low 2 long press
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::DecrementLong);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += LONG_PRESS_REPEAT_DELAY - 1;

    // Low 2
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1;

    // Low 2 long press repeat
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::DecrementHold);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    // Fall 1 → reset
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Reset);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Rise 1
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Rise 2
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Both released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Nothing);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());

    time += 1000;

    // Both falling
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(time));
    EXPECT_EQ(b.update(), IncrementDecrementButtons::Reset);

    Mock::VerifyAndClearExpectations(&ArduinoMock::getInstance());
}