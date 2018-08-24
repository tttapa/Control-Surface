#include <gmock/gmock.h>

#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>

using namespace ::testing;
using namespace ExtIO;

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

TEST(ExtendedInputOutput, digitalRead) {
    MockExtIOElement el_1 = {10};
    MockExtIOElement el_2 = {10};

    Sequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(0)).InSequence(seq);
    digitalRead(0);

    EXPECT_CALL(el_1, digitalRead(0)).InSequence(seq);
    digitalRead(el_1.pin(0));
    EXPECT_CALL(el_1, digitalRead(9)).InSequence(seq);
    digitalRead(el_1.pin(9));

    EXPECT_CALL(el_2, digitalRead(0)).InSequence(seq);
    digitalRead(el_2.pin(0));
    EXPECT_CALL(el_2, digitalRead(9)).InSequence(seq);
    digitalRead(el_2.pin(9));

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}