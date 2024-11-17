#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Display/DisplayInterface.hpp>
#include <Display/MCU/LCDDisplay.hpp>
#include <MIDI_Inputs/MCU/LCD.hpp>

USING_CS_NAMESPACE;

TEST(LCD, Ranges) {
    { // Overlapping ranges
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 'c', 'd', 0xF7,
        };
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "abcd");
        EXPECT_TRUE(lcd.getDirty());
    }
    { // Buffer longer, MIDI in middle of buffer
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x01, 'b', 'c', 0xF7,
        };
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), " bc ");
        EXPECT_TRUE(lcd.getDirty());
    }
    { // MIDI overlaps with start of buffer
        MCU::LCD<4> lcd(1);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 0xF7,
        };
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "b   ");
        EXPECT_TRUE(lcd.getDirty());
    }
    { // MIDI overlaps with end of buffer
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x03, 'd', 'e', 0xF7,
        };
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "   d");
        EXPECT_TRUE(lcd.getDirty());
    }
    { // MIDI longer
        MCU::LCD<4> lcd(1);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00,
            'a',  'b',  'c',  'd',  'e',  'f',  0xF7,
        };
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "bcde");
        EXPECT_TRUE(lcd.getDirty());
    }
    { // No overlap, buffer lower
        MCU::LCD<4> lcd(0);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x04, 'e', 'f', 'g', 'h', 0xF7,
        };
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "    ");
        EXPECT_FALSE(lcd.getDirty());
        EXPECT_FALSE(lcd.getDirty());
    }
    { // No overlap, MIDI lower
        MCU::LCD<4> lcd(4);
        std::vector<uint8_t> sysex = {
            0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 'c', 'd', 0xF7,
        };
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_STREQ(lcd.getText(), "    ");
        EXPECT_FALSE(lcd.getDirty());
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
    for (auto sysex : sysexs) {
        lcd.clearDirty();
        MIDIInputElementSysEx::updateAllWith(sysex);
        EXPECT_TRUE(lcd.getDirty());
    }
    EXPECT_STREQ(lcd.getText(), "abcd"
                                "efgh"
                                "ijkl"
                                "mnop");
}

TEST(LCD, FillManyBuffersWithOneMIDIMessage) {
    MCU::LCD<4> lcds[] = {0x0, 0x4, 0x8, 0xC};
    for (auto &lcd : lcds) lcd.clearDirty();
    std::vector<uint8_t> sysex = {
        0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, 'a', 'b', 'c', 'd', 'e',
        'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm', 'n', 'o', 'p', 0xF7,
    };
    MIDIInputElementSysEx::updateAllWith(sysex);
    EXPECT_STREQ(lcds[0].getText(), "abcd");
    EXPECT_STREQ(lcds[1].getText(), "efgh");
    EXPECT_STREQ(lcds[2].getText(), "ijkl");
    EXPECT_STREQ(lcds[3].getText(), "mnop");
    for (auto &lcd : lcds) EXPECT_TRUE(lcd.getDirty());
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

struct TestDisplay : DisplayInterface {
    MOCK_METHOD(void, begin, (), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(void, drawBackground, (), (override));
    MOCK_METHOD(void, display, (), (override));
    MOCK_METHOD(void, drawPixel, (int16_t, int16_t, uint16_t), (override));
    MOCK_METHOD(void, setTextColor, (uint16_t), (override));
    MOCK_METHOD(void, setTextSize, (uint8_t), (override));
    MOCK_METHOD(void, setCursor, (int16_t, int16_t), (override));
    MOCK_METHOD(size_t, write, (uint8_t), (override));
    MOCK_METHOD(size_t, print, (std::string_view), ());
    MOCK_METHOD(void, drawLine, (int16_t, int16_t, int16_t, int16_t, uint16_t),
                (override));
    MOCK_METHOD(void, drawFastVLine, (int16_t, int16_t, int16_t, uint16_t),
                (override));
    MOCK_METHOD(void, drawFastHLine, (int16_t, int16_t, int16_t, uint16_t),
                (override));
    MOCK_METHOD(void, drawXBitmap,
                (int16_t, int16_t, const uint8_t[], int16_t, int16_t, uint16_t),
                (override));
    MOCK_METHOD(void, fillRect, (int16_t, int16_t, int16_t, int16_t, uint16_t),
                (override));
    MOCK_METHOD(void, drawCircle, (int16_t, int16_t, int16_t, uint16_t),
                (override));
    MOCK_METHOD(void, fillCircle, (int16_t, int16_t, int16_t, uint16_t),
                (override));
    size_t print(const char s[]) override {
        return print(std::string_view {s});
    }
};

TEST(LCDDisplay, multiple) {
    using namespace std::string_view_literals;
    MCU::LCD<> lcd(0);
    std::vector<uint8_t> sysex = {
        0xF0, 0x00, 0x00, 0x66, 0x10, 0x12, 0x00, //
        'a',  'b',  'c',  'd',  'e',  'f',  ' ',  //
        'h',  'i',  'j',  'k',  'l',  'm',  ' ',  //
        0xF7,
    };
    lcd.clearDirty();
    testing::NiceMock<TestDisplay> displays[2];
    MCU::LCDDisplay lcd_displays[] {
        {displays[0], lcd, 1, 1, {0, 0}, 2, 0xFFFF},
        {displays[1], lcd, 2, 1, {0, 0}, 2, 0xFFFF},
    };
    lcd.begin();
    for (auto &lcd_display : lcd_displays) EXPECT_TRUE(lcd_display.getDirty());
    EXPECT_CALL(displays[0], print("      "sv));
    lcd_displays[0].draw();
    EXPECT_TRUE(lcd_displays[1].getDirty());
    EXPECT_CALL(displays[1], print("      "sv));
    lcd_displays[1].draw();
    for (auto &lcd_display : lcd_displays) EXPECT_FALSE(lcd_display.getDirty());
    MIDIInputElementSysEx::updateAllWith(sysex);
    std::string_view content {lcd.getText(), 14};
    EXPECT_EQ(content, "abcdef hijklm "sv);
    EXPECT_TRUE(lcd.getDirty());
    for (auto &lcd_display : lcd_displays) EXPECT_TRUE(lcd_display.getDirty());
    EXPECT_CALL(displays[0], print("abcdef"sv));
    lcd_displays[0].draw();
    EXPECT_TRUE(lcd_displays[1].getDirty());
    EXPECT_CALL(displays[1], print("hijklm"sv));
    lcd_displays[1].draw();
    for (auto &lcd_display : lcd_displays) EXPECT_FALSE(lcd_display.getDirty());
}
