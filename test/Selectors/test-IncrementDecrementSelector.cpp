#include <MockSelectable.hpp>
#include <Selectors/IncrementDecrementSelector.hpp>

using namespace ::testing;
using namespace CS;

using AH::Updatable;

TEST(IncrementDecrementSelector, pressIncrementWrap) {
    MockSelectable<3> selectable;

    IncrementDecrementSelector<3> selector = {selectable, {2, 3}, Wrap::Wrap};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(1));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(2));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementDecrementSelector, pressDecrementWrap) {
    MockSelectable<3> selectable;

    IncrementDecrementSelector<3> selector = {selectable, {3, 2}, Wrap::Wrap};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(2));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(1));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementDecrementSelector, reset) {
    MockSelectable<10> selectable;

    IncrementDecrementSelector<10> selector = {selectable, {2, 3}, Wrap::Wrap};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::beginAll();

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

    Updatable<>::updateAll();
}

TEST(IncrementDecrementSelector, pressIncrementClamp) {
    MockSelectable<3> selectable;

    IncrementDecrementSelector<3> selector = {selectable, {2, 3}, Wrap::Clamp};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::beginAll();

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

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(IncrementDecrementSelector, pressDecrementClamp) {
    MockSelectable<3> selectable;

    IncrementDecrementSelector<3> selector = {selectable, {3, 2}, Wrap::Clamp};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::beginAll();

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

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}