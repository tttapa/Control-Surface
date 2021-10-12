#include <gtest/gtest.h>

#include <MIDI_Inputs/MCU/LCD.hpp>

USING_CS_NAMESPACE;

TEST(LCD, Ranges) {
    { // Overlapping ranges
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 'c', 'd', 0xF7,
        };
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "abcd");
    }
    { // Buffer longer, MIDI in middle of buffer
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x01, 'b', 'c', 0xF7,
        };
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), " bc ");
    }
    { // MIDI overlaps with start of buffer
        MCU::LCD<4> lcd(1);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 0xF7,
        };
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "b   ");
    }
    { // MIDI overlaps with end of buffer
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x03, 'd', 'e', 0xF7,
        };
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "   d");
    }
    { // MIDI longer
        MCU::LCD<4> lcd(1);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00,
            'a',  'b',  'c',  'd',  'e',  'f',  0xF7,
        };
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "bcde");
    }
    { // No overlap, buffer lower
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x04, 'e', 'f', 'g', 'h', 0xF7,
        };
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "    ");
    }
    { // No overlap, MIDI lower
        MCU::LCD<4> lcd(4);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 'c', 'd', 0xF7,
        };
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "    ");
    }
}

TEST(LCD, FillOneBufferWithManyMIDIMessages) {
    MCU::LCD<16> lcd(0);
    std::vector<uint8_t> sysexs[] = {
        {0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 'c', 'd', 0xF7},
        {0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x04, 'e', 'f', 'g', 'h', 0xF7},
        {0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x08, 'i', 'j', 'k', 'l', 0xF7},
        {0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x0C, 'm', 'n', 'o', 'p', 0xF7},
    };
    for (auto sysex : sysexs)
        MIDIInputElementSysEx::updateAllWith(sysex);
    EXPECT_STREQ(lcd.getText(), "abcd"
                                "efgh"
                                "ijkl"
                                "mnop");
}

TEST(LCD, FillManyBuffersWithOneMIDIMessage) {
    MCU::LCD<4> lcds[] = {0x0, 0x4, 0x8, 0xC};
    std::vector<uint8_t> sysex = {
        0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 'c', 'd', 'e',
        'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm', 'n', 'o', 'p', 0xF7,
    };
    MIDIInputElementSysEx::updateAllWith(sysex);
    EXPECT_STREQ(lcds[0].getText(), "abcd");
    EXPECT_STREQ(lcds[1].getText(), "efgh");
    EXPECT_STREQ(lcds[2].getText(), "ijkl");
    EXPECT_STREQ(lcds[3].getText(), "mnop");
}

TEST(LCDlength, len) {
    auto range = {0, 1, 2, 3, 4, 5, 6, 7};
    for (int a : range) {
        for (int b : range) {
            if (b < a)
                continue;
            for (int c : range) {
                for (int d : range) {
                    if (d < c)
                        continue;
                    int l1 = b - a - max(0, c - a) - max(0, b - d);
                    int l2 = d - c - max(0, a - c) - max(0, d - b);
                    // DEBUGVAL(a, b, c, d, l1, l2);
                    EXPECT_EQ(l1, l2);
                }
            }
        }
    }
}
