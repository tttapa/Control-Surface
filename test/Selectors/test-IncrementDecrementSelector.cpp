#include <MockSelectable.hpp>
#include <Selectors/IncrementDecrementSelector.hpp>

using namespace ::testing;

TEST(IncrementDecrementSelector, pressIncrementWrap) {
    MockSelectable selectable;

    IncrementDecrementSelector selector = {selectable, 3, {2, 3}, true};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Selector::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(1));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(2));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(0));

    selector.updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementDecrementSelector, pressDecrementWrap) {
    MockSelectable selectable;

    IncrementDecrementSelector selector = {selectable, 3, {3, 2}, true};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Selector::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(2));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(1));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(0));

    selector.updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementDecrementSelector, reset) {
    MockSelectable selectable;

    IncrementDecrementSelector selector = {selectable, 10, {2, 3}, true};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Selector::beginAll();

    EXPECT_CALL(selectable, select(5));

    selector.set(5);

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(0));

    selector.updateAll();
}

TEST(IncrementDecrementSelector, pressIncrementNoWrap) {
    MockSelectable selectable;

    IncrementDecrementSelector selector = {selectable, 3, {2, 3}, false};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Selector::beginAll();

    EXPECT_CALL(selectable, select(1));

    selector.set(1);

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(2));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementDecrementSelector, pressDecrementNoWrap) {
    MockSelectable selectable;

    IncrementDecrementSelector selector = {selectable, 3, {3, 2}, false};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Selector::beginAll();

    EXPECT_CALL(selectable, select(1));

    selector.set(1);

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(0));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    selector.updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}