#include <Hardware/Button.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

/**
 * Upon construction, the button pin should be set to `INPUT_PULLUP`.
 */
TEST(Button, construct) {
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}
/**
 * If the pin state is always high, the button state should be `Released`.
 */
TEST(Button, released) {
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

/**
 * If after the debounce time, the pin state is low, the state should be
 * `Falling`.
 */
TEST(Button, falling) {
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

/**
 * If two successive readings are low, the state should be `Pressed`.
 */
TEST(Button, pressed) {
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    b.getState();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

/**
 * If a low reading is followed by a high reading (after the debounce time),
 * the state should be `Rising`.
 */
TEST(Button, rising) {
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    b.getState();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    b.getState();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Rising);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
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
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1001));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1002));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1003));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1004));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1005));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Rising);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
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
    Button b(2);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    b.begin();
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(999));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);
    // Steady low
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Rising);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1001));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1002));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1003));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1004));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1005));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_EQ(b.getState(), Button::Released);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Falling);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_EQ(b.getState(), Button::Pressed);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}