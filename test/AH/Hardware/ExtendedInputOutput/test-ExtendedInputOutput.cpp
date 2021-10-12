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
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(1));
    digitalRead(pin_t(1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2));
    digitalRead(unsigned(2));

    EXPECT_CALL(el_1, digitalRead(0));
    digitalRead(el_1.pin(0));
    EXPECT_CALL(el_1, digitalRead(9));
    digitalRead(el_1.pin(9));

    EXPECT_CALL(el_2, digitalRead(0));
    digitalRead(el_2.pin(0));
    EXPECT_CALL(el_2, digitalRead(9));
    digitalRead(el_2.pin(9));

    CachedExtIOPin c_el_1_5{el_1.pin(5)};
    EXPECT_CALL(el_1, digitalRead(5));
    digitalRead(c_el_1_5);

    digitalRead(NO_PIN);
    digitalRead(CachedExtIOPin(NO_PIN));

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, analogRead) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(0));
    analogRead(0);
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(1));
    analogRead(pin_t(1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2));
    analogRead(unsigned(2));

    EXPECT_CALL(el_1, analogRead(0));
    analogRead(el_1.pin(0));
    EXPECT_CALL(el_1, analogRead(9));
    analogRead(el_1.pin(9));

    EXPECT_CALL(el_2, analogRead(0));
    analogRead(el_2.pin(0));
    EXPECT_CALL(el_2, analogRead(9));
    analogRead(el_2.pin(9));

    CachedExtIOPin c_el_1_5{el_1.pin(5)};
    EXPECT_CALL(el_1, analogRead(5));
    analogRead(c_el_1_5);

    analogRead(NO_PIN);
    analogRead(CachedExtIOPin(NO_PIN));

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, digitalWrite) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    digitalWrite(0, HIGH);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    digitalWrite(pin_t(1), HIGH);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    digitalWrite(unsigned(2), HIGH);

    EXPECT_CALL(el_1, digitalWrite(0, HIGH));
    digitalWrite(el_1.pin(0), HIGH);
    EXPECT_CALL(el_1, digitalWrite(9, LOW));
    digitalWrite(el_1.pin(9), LOW);

    EXPECT_CALL(el_2, digitalWrite(0, LOW));
    digitalWrite(el_2.pin(0), LOW);
    EXPECT_CALL(el_2, digitalWrite(9, HIGH));
    digitalWrite(el_2.pin(9), HIGH);

    CachedExtIOPin c_el_1_5{el_1.pin(5)};
    EXPECT_CALL(el_1, digitalWrite(5, LOW));
    digitalWrite(c_el_1_5, LOW);

    digitalWrite(NO_PIN, HIGH);
    digitalWrite(CachedExtIOPin(NO_PIN), HIGH);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, analogWrite) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(0, 129));
    analogWrite(0, analog_t(129));
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(1, 128));
    analogWrite(1, 128);
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 129));
    analogWrite(pin_t(2), analog_t(129));
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(3, 128));
    analogWrite(pin_t(3), 128);
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(4, 129));
    analogWrite(unsigned(4), analog_t(129));
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(5, 128));
    analogWrite(unsigned(5), 128);

    EXPECT_CALL(el_1, analogWrite(0, 127));
    analogWrite(el_1.pin(0), analog_t(127));
    EXPECT_CALL(el_1, analogWrite(9, 126));
    analogWrite(el_1.pin(9), 126);

    EXPECT_CALL(el_2, analogWrite(0, 12));
    analogWrite(el_2.pin(0), analog_t(12));
    EXPECT_CALL(el_2, analogWrite(9, 1));
    analogWrite(el_2.pin(9), 1);

    CachedExtIOPin c_el_1_5{el_1.pin(5)};
    EXPECT_CALL(el_1, analogWrite(5, 125));
    analogWrite(c_el_1_5, analog_t(125));
    EXPECT_CALL(el_1, analogWrite(5, 124));
    analogWrite(c_el_1_5, 124);

    analogWrite(NO_PIN, 123);
    analogWrite(CachedExtIOPin(NO_PIN), 123);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, pinMode) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, INPUT_PULLUP));
    pinMode(0, INPUT_PULLUP);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(1, INPUT_PULLUP));
    pinMode(pin_t(1), INPUT_PULLUP);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    pinMode(unsigned(2), INPUT_PULLUP);

    EXPECT_CALL(el_1, pinMode(0, INPUT_PULLUP));
    pinMode(el_1.pin(0), INPUT_PULLUP);
    EXPECT_CALL(el_1, pinMode(9, OUTPUT));
    pinMode(el_1.pin(9), OUTPUT);

    EXPECT_CALL(el_2, pinMode(0, INPUT));
    pinMode(el_2.pin(0), INPUT);
    EXPECT_CALL(el_2, pinMode(9, INPUT_PULLUP));
    pinMode(el_2.pin(9), INPUT_PULLUP);

    CachedExtIOPin c_el_1_5{el_1.pin(5)};
    EXPECT_CALL(el_1, pinMode(5, OUTPUT));
    pinMode(c_el_1_5, OUTPUT);

    pinMode(NO_PIN, OUTPUT);
    pinMode(CachedExtIOPin(NO_PIN), OUTPUT);

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

TEST(ExtendedInputOutput, shiftOutNative) {
    MockExtIOElement el = {10};

    pin_t data = 0;
    pin_t clck = 1;

    EXPECT_CALL(ArduinoMock::getInstance(),
                shiftOut(0, 1, LSBFIRST, 0b00111001));
    shiftOut(data, clck, LSBFIRST, 0b00111001);
}

TEST(ExtendedInputOutput, shiftOutNativeInt) {
    MockExtIOElement el = {10};

    int data = 0;
    int clck = 1;

    EXPECT_CALL(ArduinoMock::getInstance(),
                shiftOut(0, 1, LSBFIRST, 0b00111001));
    shiftOut(data, clck, LSBFIRST, 0b00111001);
}

TEST(ExtendedInputOutput, shiftOutNativeUInt) {
    MockExtIOElement el = {10};

    unsigned data = 0;
    unsigned clck = 1;

    EXPECT_CALL(ArduinoMock::getInstance(),
                shiftOut(0, 1, LSBFIRST, 0b00111001));
    shiftOut(data, clck, LSBFIRST, 0b00111001);
}

TEST(ExtendedInputOutput, shiftOutMSBFIRSTMixed) {
    MockExtIOElement el = {10};

    testing::InSequence s;
    uint8_t data = 0;
    uint8_t clck = 1;

    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));

    shiftOut(el.pin(data), pin_t(clck), MSBFIRST, 0b10011100);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, shiftOutLSBFIRSTMixed) {
    MockExtIOElement el = {10};

    testing::InSequence s;
    uint8_t data = 0;
    uint8_t clck = 1;

    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));

    shiftOut(el.pin(data), pin_t(clck), LSBFIRST, 0b00111001);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, shiftOutNoPin) {
    MockExtIOElement el = {10};

    pin_t data = 0;
    pin_t clck = NO_PIN;

    shiftOut(data, clck, LSBFIRST, 0b00111001);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, shiftOutMSBFIRSTMixedCached) {
    MockExtIOElement el = {10};

    testing::InSequence s;
    uint8_t data = 0;
    uint8_t clck = 1;

    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));

    shiftOut(CachedExtIOPin(el.pin(data)), CachedExtIOPin(pin_t(clck)),
             MSBFIRST, 0b10011100);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, shiftOutLSBFIRSTMixedCached) {
    MockExtIOElement el = {10};

    testing::InSequence s;
    uint8_t data = 0;
    uint8_t clck = 1;

    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));
    EXPECT_CALL(el, digitalWrite(data, 0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 1));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(clck, 0));

    shiftOut(CachedExtIOPin(el.pin(data)), CachedExtIOPin(pin_t(clck)),
             LSBFIRST, 0b00111001);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, shiftOutMSBFIRSTCached) {
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

    shiftOut(CachedExtIOPin(el.pin(data)), CachedExtIOPin(el.pin(clck)),
             MSBFIRST, 0b10011100);
}

TEST(ExtendedInputOutput, shiftOutLSBFIRSTCached) {
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

    shiftOut(CachedExtIOPin(el.pin(data)), CachedExtIOPin(el.pin(clck)),
             LSBFIRST, 0b00111001);
}

TEST(ExtendedInputOutput, shiftOutNativeCached) {
    MockExtIOElement el = {10};

    pin_t data = 0;
    pin_t clck = 1;

    EXPECT_CALL(ArduinoMock::getInstance(),
                shiftOut(0, 1, LSBFIRST, 0b00111001));
    shiftOut(CachedExtIOPin(data), CachedExtIOPin(clck), LSBFIRST, 0b00111001);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(ExtendedInputOutput, shiftOutNoPinCached) {
    MockExtIOElement el = {10};

    pin_t data = 0;
    pin_t clck = NO_PIN;

    shiftOut(CachedExtIOPin(data), CachedExtIOPin(clck), LSBFIRST, 0b00111001);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
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