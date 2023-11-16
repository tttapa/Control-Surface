#include <MIDI_Outputs/TouchpadPiano.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace cs;

TEST(TouchpadPianoStandard, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    TouchpadPiano piano(3, 2, {0x3C, Channel_7, Cable_13}, PianoMode::Standard);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    piano.begin();

    /* TODO */

    // Reading
    // EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, LOW));
    // EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2));
    // EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, HIGH));
    // EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(60));
    // piano.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}
