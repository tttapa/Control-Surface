#include <gmock/gmock.h>

#include <AH/Timing/MillisMicrosTimer.hpp>

USING_AH_NAMESPACE;

using namespace testing;

TEST(MillisMicrosTimer, TimerMillis) {
    Timer<millis> timer = 3;
    InSequence s;
    EXPECT_CALL(ArduinoMock::getInstance(), millis).WillOnce(Return(100));
    timer.begin();
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_CALL(ArduinoMock::getInstance(), millis)
        .WillOnce(Return(100))
        .WillOnce(Return(101))
        .WillOnce(Return(102))
        .WillOnce(Return(103))
        .WillOnce(Return(104))
        .WillOnce(Return(105))
        .WillOnce(Return(106))
        .WillOnce(Return(107))
        .WillOnce(Return(108))
        .WillOnce(Return(109));
    for (unsigned i = 0; i < 10; ++i)
        EXPECT_EQ(!!timer, i % 3 == 0) << "i = " << i;
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MillisMicrosTimer, TimerMicros) {
    Timer<micros> timer = 3;
    InSequence s;
    EXPECT_CALL(ArduinoMock::getInstance(), micros).WillOnce(Return(100));
    timer.begin();
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_CALL(ArduinoMock::getInstance(), micros)
        .WillOnce(Return(100))
        .WillOnce(Return(101))
        .WillOnce(Return(102))
        .WillOnce(Return(103))
        .WillOnce(Return(104))
        .WillOnce(Return(105))
        .WillOnce(Return(106))
        .WillOnce(Return(107))
        .WillOnce(Return(108))
        .WillOnce(Return(109));
    for (unsigned i = 0; i < 10; ++i)
        EXPECT_EQ(!!timer, i % 3 == 0) << "i = " << i;
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}