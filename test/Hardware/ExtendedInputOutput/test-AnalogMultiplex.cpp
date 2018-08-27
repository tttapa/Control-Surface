#include <gtest/gtest.h>

#include <Hardware/ExtendedInputOutput/AnalogMultiplex.h>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <Helpers/Debug.hpp>

TEST(AnalogMultiplex, analogReadNoEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}};

    DEBUGFN(mux.getStart());
    DEBUGFN(mux.getEnd());

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    mux.begin();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(2);
    ExtIO::analogRead(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, digitalReadNoEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    mux.begin();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(A0));
    ExtIO::digitalRead(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, analogReadEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}, 6};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    mux.begin();

    ::testing::Sequence seq;
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, LOW))
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0))
        .Times(2)
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH))
        .InSequence(seq);
    ExtIO::analogRead(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, digitalReadEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}, 6};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    mux.begin();

    ::testing::Sequence seq;
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, LOW))
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(A0)).InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH))
        .InSequence(seq);
    ExtIO::digitalRead(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}