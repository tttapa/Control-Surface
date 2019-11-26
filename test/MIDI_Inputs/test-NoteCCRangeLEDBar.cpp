#include <gtest-wrapper.h>

#include <MIDI_Inputs/LEDs/NoteCCRangeLEDBar.hpp>

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
    ChannelMessageMatcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x46};
    MIDIInputElementNote::updateAllWith(midimsg1);
    testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    for (int i = 0; i < 8; ++i)
        EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(i, i < 5));
    ChannelMessageMatcher midimsg2 = {0x90, CHANNEL_5, 0x3C, 0x47};
    MIDIInputElementNote::updateAllWith(midimsg2);
    testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}