#include <gtest/gtest.h>

#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

USING_AH_NAMESPACE;

TEST(AnalogMultiplex, analogReadNoEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}};

    DEBUGFN(mux.getStart());
    DEBUGFN(mux.getEnd());

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
    ExtIO::analogRead(mux.pin(0b0000));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(2);
    ExtIO::analogRead(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(2);
    ExtIO::analogRead(mux.pin(0b1010));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, analogReadNoEnableNoDummyRead) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}};
    mux.discardFirstReading(false);
    DEBUGFN(mux.getStart());
    DEBUGFN(mux.getEnd());

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
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(1);
    ExtIO::analogRead(mux.pin(0b0000));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(1);
    ExtIO::analogRead(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0)).Times(1);
    ExtIO::analogRead(mux.pin(0b1010));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, digitalReadNoEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    ExtendedIOElement::beginAll();

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
    ExtendedIOElement::beginAll();

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

TEST(AnalogMultiplex, analogReadBufferedEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}, 6};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    ExtendedIOElement::beginAll();

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
    ExtIO::analogReadBuffered(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, analogReadEnable3) {
    AnalogMultiplex<3> mux = {A0, {2, 3, 4}, 6};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    ExtendedIOElement::beginAll();

    ::testing::Sequence seq;
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, LOW))
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0))
        .Times(2)
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH))
        .InSequence(seq);
    ExtIO::analogRead(mux.pin(0b111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, LOW))
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0))
        .Times(2)
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH))
        .InSequence(seq);
    ExtIO::analogRead(mux.pin(0b000));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, LOW))
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0))
        .Times(2)
        .InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH))
        .InSequence(seq);
    ExtIO::analogRead(mux.pin(0b110));

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
    ExtendedIOElement::beginAll();

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

TEST(AnalogMultiplex, digitalReadBufferedEnable) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}, 6};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    ExtendedIOElement::beginAll();

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
    ExtIO::digitalReadBuffered(mux.pin(0b1111));

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, pinMode) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}, 6};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    ExtendedIOElement::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A0, INPUT_PULLUP));
    ExtIO::pinMode(mux.pin(0b1111), INPUT_PULLUP);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(AnalogMultiplex, pinModeBuffered) {
    AnalogMultiplex<4> mux = {A0, {2, 3, 4, 5}, 6};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    ExtendedIOElement::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A0, INPUT_PULLUP));
    ExtIO::pinModeBuffered(mux.pin(0b1111), INPUT_PULLUP);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}