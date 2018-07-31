#include <MockSelectable.hpp>
#include <Selectors/ManyButtonsSelector.hpp>

using namespace ::testing;

TEST(ManyButtonsSelector, press) {
    MockSelectable<4> selectable;

    ManyButtonsSelector<4> selector = {selectable, {2, 3, 4, 5}};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, INPUT_PULLUP));

    EXPECT_CALL(selectable, select(0));

    Updatable::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(4))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .WillOnce(Return(HIGH));

    EXPECT_CALL(selectable, select(1));

    Updatable::updateAll();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}