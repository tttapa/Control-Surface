#include <gtest/gtest.h>

#include <MIDI_Inputs/LEDs/NoteCCKPLEDBar.hpp>

USING_CS_NAMESPACE;

TEST(NoteLEDBar, NoteLEDBar) {
    NoteLEDBar<8> mn{
        {{0, 1, 2, 3, 4, 5, 6, 7}},
        {0x3C, CHANNEL_5},
    };
    for (int i = 0; i < 8; ++i) {
        EXPECT_CALL(ArduinoMock::getInstance(), pinMode(i, OUTPUT));
        EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(i, LOW));
    }
    mn.begin();
    testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    for (int i = 0; i < 8; ++i)
        EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(i, i < 4));
    ChannelMessage midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3C,
        0x47,
    };
    MIDIInputElementNote::updateAllWith(midimsg1);
    testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    for (int i = 0; i < 8; ++i)
        EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(i, i < 5));
    ChannelMessage midimsg2 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3C,
        0x48,
    };
    MIDIInputElementNote::updateAllWith(midimsg2);
    testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}