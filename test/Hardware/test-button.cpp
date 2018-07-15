#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Hardware/Button.h>

using namespace ::testing;

/**
 * Upon construction, the button pin should be set to `INPUT_PULLUP`.
 */
TEST(Button, construct) {
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    Button b(2);
}
/** 
 * If the pin state is always high, the button state should be `Released`.
 */
TEST(Button, released) {
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
}

/** 
 * If after the debounce time, the pin state is low, the state should be 
 * `Falling`.
 */
TEST(Button, falling) {
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);
}

/** 
 * If two successive readings are low, the state should be `Pressed`.
 */
TEST(Button, pressed) {
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    b.getState();
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
}

/** 
 * If a low reading is followed by a high reading (after the debounce time),
 * the state should be `Rising`.
 */
TEST(Button, rising) {
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    b.getState();
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    b.getState();
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Rising);
}

/** 
 * When the button bounces when pressing, the state should stay pressed.
 * ```
 * Raw input:
 *    HIGH  ─┐ ┌─┐      ┌─
 *    LOW    └─┘ └──────┘ 
 *           ├─┼─┼────┤
 *
 * Debounced output:
 *    HIGH  ─┐          ┌─
 *    LOW    └──────────┘ 
 * ```
 */
TEST(Button, debouncePressed) {
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1001));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1002));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1003));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1004));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1005));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Rising);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
}

/** 
 * When the button bounces when releasing, the state should stay released.
 * ```
 * Raw input:
 *    HIGH   ┌─┐ ┌──────┐ 
 *    LOW   ─┘ └─┘      └─
 *           ├─┼─┼────┤
 *
 * Debounced output:
 *    HIGH   ┌──────────┐
 *    LOW   ─┘          └─
 * ```
 */
TEST(Button, debounceReleased) {
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(999));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);
    // Steady low
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Rising);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1001));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1002));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1003));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1004));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(1005));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
}