#include <MockSelectable.hpp>
#include <Selectors/IncrementSelector.hpp>

using namespace ::testing;

TEST(IncrementSelector, pressWrap) {
    MockSelectable selectable;

    IncrementSelector selector = {selectable, 3, IncrementButton(2), true};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Selector::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(1));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(2));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(5000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(6000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(0));

    selector.updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementSelector, pressNoWrap) {
    MockSelectable selectable;

    IncrementSelector selector = {selectable, 3, IncrementButton(2), false};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Selector::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(1));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(2));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(5000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(6000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    selector.updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}