#include <MockSelectable.hpp>
#include <Selectors/ManyButtonsSelector.hpp>

using namespace ::testing;

USING_CS_NAMESPACE;

using AH::Updatable;

TEST(ManyButtonsSelector, press) {
    MockSelectable<4> selectable;

    ManyButtonsSelector<4> selector = {selectable, {2, 3, 4, 5}};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable<>::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillRepeatedly(Return(1000));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillRepeatedly(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillRepeatedly(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(4))
        .WillRepeatedly(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .WillRepeatedly(Return(LOW));

    EXPECT_CALL(selectable, select(3));

    Updatable<>::updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// TODO: add test for multiple buttons at the same time