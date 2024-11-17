#include <MockSelectable.hpp>
#include <Selectors/IncrementSelector.hpp>

using namespace ::testing;

USING_CS_NAMESPACE;

using AH::Updatable;

TEST(IncrementSelector, pressWrap) {
    MockSelectable<3> selectable;

    IncrementSelector<3> selector = {selectable, AH::IncrementButton(2)};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(1));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(2));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(5000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));

    Updatable<>::updateAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(6000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}
