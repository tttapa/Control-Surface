#include <gmock/gmock.h>

#include <AH/Hardware/ExtendedInputOutput/ExtendedIOElement.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <type_traits>

using namespace ::testing;
USING_AH_NAMESPACE;
using namespace ExtIO;

class MockExtIOElement : public ExtendedIOElement {
  public:
    MockExtIOElement(pin_t length) : ExtendedIOElement(length) {}

    MOCK_METHOD(void, pinMode, (pin_t, uint8_t), (override));
    MOCK_METHOD(void, digitalWrite, (pin_t, uint8_t), (override));
    MOCK_METHOD(PinStatus_t, digitalRead, (pin_t), (override));
    MOCK_METHOD(analog_t, analogRead, (pin_t), (override));
    MOCK_METHOD(void, analogWrite, (pin_t, analog_t), (override));

    MOCK_METHOD(void, pinModeBuffered, (pin_t, uint8_t), (override));
    MOCK_METHOD(void, digitalWriteBuffered, (pin_t, uint8_t), (override));
    MOCK_METHOD(PinStatus_t, digitalReadBuffered, (pin_t), (override));
    MOCK_METHOD(analog_t, analogReadBuffered, (pin_t), (override));
    MOCK_METHOD(void, analogWriteBuffered, (pin_t, analog_t), (override));

    MOCK_METHOD(void, begin, (), (override));
    MOCK_METHOD(void, updateBufferedOutputs, (), (override));
    MOCK_METHOD(void, updateBufferedInputs, (), (override));
};

class MinimalMockExtIOElement : public ExtendedIOElement {
  public:
    MinimalMockExtIOElement(pin_t length) : ExtendedIOElement(length) {}

    MOCK_METHOD(void, pinModeBuffered, (pin_t, uint8_t), (override));
    MOCK_METHOD(void, digitalWriteBuffered, (pin_t, uint8_t), (override));
    MOCK_METHOD(PinStatus_t, digitalReadBuffered, (pin_t), (override));
    MOCK_METHOD(analog_t, analogReadBuffered, (pin_t), (override));
    MOCK_METHOD(void, analogWriteBuffered, (pin_t, analog_t), (override));

    MOCK_METHOD(void, begin, (), (override));
    MOCK_METHOD(void, updateBufferedOutputs, (), (override));
    MOCK_METHOD(void, updateBufferedInputs, (), (override));
};

TEST(ExtendedInputOutput, ExtendedIOElement) {
    MockExtIOElement el_1 = {10};
    EXPECT_EQ(el_1.getLength(), 10);
    EXPECT_EQ(el_1[2], el_1.pin(2));
}

TEST(ExtendedInputOutput, digitalRead) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(0));
    digitalRead(0);

    EXPECT_CALL(el_1, digitalRead(0));
    digitalRead(el_1.pin(0));
    EXPECT_CALL(el_1, digitalRead(9));
    digitalRead(el_1.pin(9));

    EXPECT_CALL(el_2, digitalRead(0));
    digitalRead(el_2.pin(0));
    EXPECT_CALL(el_2, digitalRead(9));
    digitalRead(el_2.pin(9));

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, analogRead) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(0));
    analogRead(0);

    EXPECT_CALL(el_1, analogRead(0));
    analogRead(el_1.pin(0));
    EXPECT_CALL(el_1, analogRead(9));
    analogRead(el_1.pin(9));

    EXPECT_CALL(el_2, analogRead(0));
    analogRead(el_2.pin(0));
    EXPECT_CALL(el_2, analogRead(9));
    analogRead(el_2.pin(9));

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, digitalWrite) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    digitalWrite(0, HIGH);

    EXPECT_CALL(el_1, digitalWrite(0, HIGH));
    digitalWrite(el_1.pin(0), HIGH);
    EXPECT_CALL(el_1, digitalWrite(9, HIGH));
    digitalWrite(el_1.pin(9), HIGH);

    EXPECT_CALL(el_2, digitalWrite(0, HIGH));
    digitalWrite(el_2.pin(0), HIGH);
    EXPECT_CALL(el_2, digitalWrite(9, HIGH));
    digitalWrite(el_2.pin(9), HIGH);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, analogWrite) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(0, 127));
    analogWrite(0, 127);

    EXPECT_CALL(el_1, analogWrite(0, 127));
    analogWrite(el_1.pin(0), 127);
    EXPECT_CALL(el_1, analogWrite(9, 127));
    analogWrite(el_1.pin(9), 127);

    EXPECT_CALL(el_2, analogWrite(0, 127));
    analogWrite(el_2.pin(0), 127);
    EXPECT_CALL(el_2, analogWrite(9, 127));
    analogWrite(el_2.pin(9), 127);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, pinMode) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, INPUT_PULLUP));
    pinMode(0, INPUT_PULLUP);

    EXPECT_CALL(el_1, pinMode(0, INPUT_PULLUP));
    pinMode(el_1.pin(0), INPUT_PULLUP);
    EXPECT_CALL(el_1, pinMode(9, INPUT_PULLUP));
    pinMode(el_1.pin(9), INPUT_PULLUP);

    EXPECT_CALL(el_2, pinMode(0, INPUT_PULLUP));
    pinMode(el_2.pin(0), INPUT_PULLUP);
    EXPECT_CALL(el_2, pinMode(9, INPUT_PULLUP));
    pinMode(el_2.pin(9), INPUT_PULLUP);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, pinOutOfBoundsGt) {
    MockExtIOElement el = {10};
    el.pin(9);
    try {
        el.pin(10);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x4567);
    }
}

TEST(ExtendedInputOutput, pinOutOfBoundsLt) {
    MockExtIOElement el = {10};
    el.pin(0);
    try {
        el.pin(-1);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x4567);
    }
}

TEST(ExtendedInputOutput, nonExistentPin) {
    MockExtIOElement el = {10};
    pin_t pin = el.pin(9) + 1;
    try {
        digitalRead(pin);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x8888);
    }
}

TEST(ExtendedInputOutput, nonExistentPinOfDeletedElement) {
    MockExtIOElement el_1 = {10};
    pin_t pin;
    {
        MockExtIOElement el_2 = {10};
        pin = el_2.pin(0);
    }
    MockExtIOElement el_3 = {10};
    try {
        digitalRead(pin);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x8888);
    }
}

TEST(ExtendedInputOutput, outOfPinNumbers) {
    pin_t max = std::numeric_limits<pin_t>::max();
    try {
        MockExtIOElement el_1 = {max};
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x00FF);
    }
}

TEST(ExtendedInputOutput, shiftOutMSBFIRST) {
    MockExtIOElement el = {10};

    testing::InSequence s;
    uint8_t data = 0;
    uint8_t clck = 1;

    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));

    shiftOut(el.pin(data), el.pin(clck), MSBFIRST, 0b10011100);
}

TEST(ExtendedInputOutput, shiftOutLSBFIRST) {
    MockExtIOElement el = {10};

    testing::InSequence s;
    uint8_t data = 0;
    uint8_t clck = 1;

    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(el, digitalWrite(clck, 1));
    EXPECT_CALL(el, digitalWrite(clck, 0));

    shiftOut(el.pin(data), el.pin(clck), LSBFIRST, 0b00111001);
}

TEST(ExtendedIOElement, bufferedMinimal) {
    MinimalMockExtIOElement e(8);

    testing::InSequence s;

    EXPECT_CALL(e, updateBufferedInputs());
    EXPECT_CALL(e, digitalReadBuffered(4));
    ExtIO::digitalRead(e.pin(4));
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, updateBufferedInputs());
    EXPECT_CALL(e, analogReadBuffered(7));
    ExtIO::analogRead(e.pin(7));
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, pinModeBuffered(0, INPUT));
    EXPECT_CALL(e, updateBufferedOutputs());
    ExtIO::pinMode(e.pin(0), INPUT);
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, digitalWriteBuffered(0, HIGH));
    EXPECT_CALL(e, updateBufferedOutputs());
    ExtIO::digitalWrite(e.pin(0), HIGH);
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, analogWriteBuffered(0, 511));
    EXPECT_CALL(e, updateBufferedOutputs());
    ExtIO::analogWrite(e.pin(0), 511);
    Mock::VerifyAndClear(&e);
}

TEST(ExtendedIOElement, bufferedGlobal) {
    MinimalMockExtIOElement e(8);

    testing::InSequence s;

    EXPECT_CALL(e, digitalReadBuffered(4));
    ExtIO::digitalReadBuffered(e.pin(4));
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, analogReadBuffered(7));
    ExtIO::analogReadBuffered(e.pin(7));
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, pinModeBuffered(0, INPUT));
    ExtIO::pinModeBuffered(e.pin(0), INPUT);
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, digitalWriteBuffered(0, HIGH));
    ExtIO::digitalWriteBuffered(e.pin(0), HIGH);
    Mock::VerifyAndClear(&e);

    EXPECT_CALL(e, analogWriteBuffered(0, 511)).Times(2);
    ExtIO::analogWriteBuffered(e.pin(0), 511);
    ExtIO::analogWriteBuffered(e.pin(0), (analog_t)511);
    Mock::VerifyAndClear(&e);
}

TEST(ExtendedIOElement, bufferedGlobalArduino) {
    testing::InSequence s;

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(4));
    ExtIO::digitalReadBuffered(4);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(7));
    ExtIO::analogReadBuffered(7);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, INPUT));
    ExtIO::pinModeBuffered(0, INPUT);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    ExtIO::digitalWriteBuffered(0, HIGH);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(0, 511)).Times(2);
    ExtIO::analogWriteBuffered(0, 511);
    ExtIO::analogWriteBuffered(0, (analog_t)511);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedIOElement, updateAllBufferedInputs) {
    MockExtIOElement el1(10);
    MockExtIOElement el2(10);

    EXPECT_CALL(el1, updateBufferedInputs());
    EXPECT_CALL(el2, updateBufferedInputs());
    ExtendedIOElement::updateAllBufferedInputs();
}

TEST(ExtendedIOElement, updateAllBufferedOutputs) {
    MockExtIOElement el1(10);
    MockExtIOElement el2(10);

    EXPECT_CALL(el1, updateBufferedOutputs());
    EXPECT_CALL(el2, updateBufferedOutputs());
    ExtendedIOElement::updateAllBufferedOutputs();
}