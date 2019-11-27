#include <gmock-wrapper.h>

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <type_traits>

using namespace ::testing;
USING_AH_NAMESPACE;
using namespace ExtIO;

W_SUGGEST_OVERRIDE_OFF

class MockExtIOElement : public ExtendedIOElement {
  public:
    MockExtIOElement(pin_t length) : ExtendedIOElement(length) {}

    MOCK_METHOD2(pinMode, void(pin_t, uint8_t));
    MOCK_METHOD2(digitalWrite, void(pin_t, uint8_t));
    MOCK_METHOD1(digitalRead, int(pin_t));
    MOCK_METHOD1(analogRead, analog_t(pin_t));
    MOCK_METHOD2(analogWrite, void(pin_t, analog_t));

    MOCK_METHOD0(begin, void());
    MOCK_METHOD0(update, void());
};

W_SUGGEST_OVERRIDE_ON

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
    EXPECT_CALL(el_1, digitalRead(0));
    digitalRead((int)el_1.pin(0));
    EXPECT_CALL(el_1, digitalRead(9));
    digitalRead(el_1.pin(9));
    EXPECT_CALL(el_1, digitalRead(9));
    digitalRead((int)el_1.pin(9));

    EXPECT_CALL(el_2, digitalRead(0));
    digitalRead(el_2.pin(0));
    EXPECT_CALL(el_2, digitalRead(0));
    digitalRead((int)el_2.pin(0));
    EXPECT_CALL(el_2, digitalRead(9));
    digitalRead(el_2.pin(9));
    EXPECT_CALL(el_2, digitalRead(9));
    digitalRead((int)el_2.pin(9));

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
    EXPECT_CALL(el_1, analogRead(0));
    analogRead((int)el_1.pin(0));
    EXPECT_CALL(el_1, analogRead(9));
    analogRead(el_1.pin(9));
    EXPECT_CALL(el_1, analogRead(9));
    analogRead((int)el_1.pin(9));

    EXPECT_CALL(el_2, analogRead(0));
    analogRead(el_2.pin(0));
    EXPECT_CALL(el_2, analogRead(0));
    analogRead((int)el_2.pin(0));
    EXPECT_CALL(el_2, analogRead(9));
    analogRead(el_2.pin(9));
    EXPECT_CALL(el_2, analogRead(9));
    analogRead((int)el_2.pin(9));

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
    EXPECT_CALL(el_1, digitalWrite(0, HIGH));
    digitalWrite((int)el_1.pin(0), HIGH);
    EXPECT_CALL(el_1, digitalWrite(9, HIGH));
    digitalWrite(el_1.pin(9), HIGH);
    EXPECT_CALL(el_1, digitalWrite(9, HIGH));
    digitalWrite((int)el_1.pin(9), HIGH);

    EXPECT_CALL(el_2, digitalWrite(0, HIGH));
    digitalWrite(el_2.pin(0), HIGH);
    EXPECT_CALL(el_2, digitalWrite(0, HIGH));
    digitalWrite((int)el_2.pin(0), HIGH);
    EXPECT_CALL(el_2, digitalWrite(9, HIGH));
    digitalWrite(el_2.pin(9), HIGH);
    EXPECT_CALL(el_2, digitalWrite(9, HIGH));
    digitalWrite((int)el_2.pin(9), HIGH);

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
    EXPECT_CALL(el_1, analogWrite(0, 127));
    analogWrite((int)el_1.pin(0), 127);
    EXPECT_CALL(el_1, analogWrite(9, 127));
    analogWrite(el_1.pin(9), 127);
    EXPECT_CALL(el_1, analogWrite(9, 127));
    analogWrite((int)el_1.pin(9), 127);
    EXPECT_CALL(el_1, analogWrite(9, 127));
    analogWrite((int)el_1.pin(9), (analog_t)127);

    EXPECT_CALL(el_2, analogWrite(0, 127));
    analogWrite(el_2.pin(0), 127);
    EXPECT_CALL(el_2, analogWrite(0, 127));
    analogWrite((int)el_2.pin(0), 127);
    EXPECT_CALL(el_2, analogWrite(9, 127));
    analogWrite(el_2.pin(9), 127);
    EXPECT_CALL(el_2, analogWrite(9, 127));
    analogWrite((int)el_2.pin(9), 127);
    EXPECT_CALL(el_2, analogWrite(9, 127));
    analogWrite((int)el_2.pin(9), (analog_t)127);

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
    EXPECT_CALL(el_1, pinMode(0, INPUT_PULLUP));
    pinMode((int)el_1.pin(0), INPUT_PULLUP);
    EXPECT_CALL(el_1, pinMode(9, INPUT_PULLUP));
    pinMode(el_1.pin(9), INPUT_PULLUP);
    EXPECT_CALL(el_1, pinMode(9, INPUT_PULLUP));
    pinMode((int)el_1.pin(9), INPUT_PULLUP);

    EXPECT_CALL(el_2, pinMode(0, INPUT_PULLUP));
    pinMode(el_2.pin(0), INPUT_PULLUP);
    EXPECT_CALL(el_2, pinMode(0, INPUT_PULLUP));
    pinMode((int)el_2.pin(0), INPUT_PULLUP);
    EXPECT_CALL(el_2, pinMode(9, INPUT_PULLUP));
    pinMode(el_2.pin(9), INPUT_PULLUP);
    EXPECT_CALL(el_2, pinMode(9, INPUT_PULLUP));
    pinMode((int)el_2.pin(9), INPUT_PULLUP);

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

    shiftOut((int)el.pin(data), (int)el.pin(clck), MSBFIRST, 0b10011100);
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

    shiftOut((int)el.pin(data), (int)el.pin(clck), LSBFIRST, 0b00111001);
}