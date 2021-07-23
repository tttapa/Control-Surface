#include <gtest-wrapper.h>

#include <AH/Hardware/ExtendedInputOutput/ShiftRegisterIn.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

USING_AH_NAMESPACE;

TEST(ShiftRegisterIn, analogReadNoEnable) {
    /*
    ISSUE: disabled because shiftIn and bitRead are not defined when building
    tests

    
    ShiftRegisterIn<8> sr = {2, 3, 4, 5};


    DEBUGFN(sr.getStart());
    DEBUGFN(sr.getEnd());

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    ExtendedIOElement::beginAll();

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(2);
    ExtIO::analogRead(sr.pin(0b0000));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(2);
    ExtIO::analogRead(sr.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(2);
    ExtIO::analogRead(sr.pin(0b1010));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    */
}