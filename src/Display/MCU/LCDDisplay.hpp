#pragma once

#include <Banks/Bank.hpp>
#include <Display/DisplayElement.hpp>
#include <Display/DisplayInterface.hpp>
#include <MIDI_Inputs/MCU/LCD.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/**
 * @brief   Displays the text of the Mackie Control Universal LCD screen for a
 *          single track. Often used for track names.
 * 
 * @ingroup DisplayElements
 */
class LCDDisplay : public DisplayElement {
  public:
    /**
     * @brief   Constructor.
     * 
     * @param   display 
     *          A reference to the display that this element will be drawn to.
     * @param   lcd 
     *          A reference to the MCU LCD MIDI input element that listens for
     *          incoming MIDI display data.
     * @param   bank 
     *          The bank that determines the active track to be displayed.
     * @param   track 
     *          The track number to display [1, 8].
     * @param   loc 
     *          The location on the display where to start drawing the text.
     * @param   textSize 
     *          The font size to use for drawing the text.
     * @param   color 
     *          The color of the text to draw.
     */
    LCDDisplay(DisplayInterface &display, LCD<> &lcd, const OutputBank &bank,
               uint8_t track, PixelLocation loc, uint8_t textSize,
               uint16_t color)
        : DisplayElement(display), lcd(lcd), bank(&bank), track(track - 1),
          line(1), x(loc.x), y(loc.y), size(textSize), color(color) {}

    /**
     * @brief   Constructor.
     * 
     * @param   display 
     *          A reference to the display that this element will be drawn to.
     * @param   lcd 
     *          A reference to the MCU LCD MIDI input element that listens for
     *          incoming MIDI display data.
     * @param   bank 
     *          The bank that determines the active track to be displayed.
     * @param   track 
     *          The track number to display [1, 8].
     * @param   line
     *          The line of the MCU display to display [1, 2].
     * @param   loc 
     *          The location on the display where to start drawing the text.
     * @param   textSize 
     *          The font size to use for drawing the text.
     * @param   color 
     *          The color of the text to draw.
     */
    LCDDisplay(DisplayInterface &display, LCD<> &lcd, const OutputBank &bank,
               uint8_t track, uint8_t line, PixelLocation loc, uint8_t textSize,
               uint16_t color)
        : DisplayElement(display), lcd(lcd), bank(&bank), track(track - 1),
          line(line - 1), x(loc.x), y(loc.y), size(textSize), color(color) {}

    /**
     * @brief   Constructor.
     * 
     * @param   display 
     *          A reference to the display that this element will be drawn to.
     * @param   lcd 
     *          A reference to the MCU LCD MIDI input element that listens for
     *          incoming MIDI display data.
     * @param   track 
     *          The track number to display [1, 8].
     * @param   loc 
     *          The location on the display where to start drawing the text.
     * @param   textSize 
     *          The font size to use for drawing the text.
     * @param   color 
     *          The color of the text to draw.
     */
    LCDDisplay(DisplayInterface &display, LCD<> &lcd, uint8_t track,
               PixelLocation loc, uint8_t textSize, uint16_t color)
        : DisplayElement(display), lcd(lcd), track(track - 1), line(1),
          x(loc.x), y(loc.y), size(textSize), color(color) {}

    /**
     * @brief   Constructor.
     * 
     * @param   display 
     *          A reference to the display that this element will be drawn to.
     * @param   lcd 
     *          A reference to the MCU LCD MIDI input element that listens for
     *          incoming MIDI display data.
     * @param   track 
     *          The track number to display [1, 8].
     * @param   line
     *          The line of the MCU display to display [1, 2].
     * @param   loc 
     *          The location on the display where to start drawing the text.
     * @param   textSize 
     *          The font size to use for drawing the text.
     * @param   color 
     *          The color of the text to draw.
     */
    LCDDisplay(DisplayInterface &display, LCD<> &lcd, uint8_t track,
               uint8_t line, PixelLocation loc, uint8_t textSize,
               uint16_t color)
        : DisplayElement(display), lcd(lcd), track(track - 1), line(line - 1),
          x(loc.x), y(loc.y), size(textSize), color(color) {}

    void draw() override {
        // If it's a message across all tracks, don't display anything.
        if (!separateTracks())
            return;

        // Determine the track and line to display
        uint8_t offset = bank ? bank->getOffset() + track : track;
        if (offset > 7)
            ERROR(F("Track number out of bounds (") << offset << ')', 0xBA41);
        if (line > 1)
            ERROR(F("Line number out of bounds (") << line << ')', 0xBA42);

        // Extract the six-character substring for this track.
        const char *text = lcd.getText() + 7 * offset + 56 * line;
        char buffer[7];
        strncpy(buffer, text, 6);
        buffer[6] = '\0';
        // Print it to the display
        display.setCursor(x, y);
        display.setTextSize(size);
        display.setTextColor(color);
        display.print(buffer);
        lcd.clearDirty();
    }

    bool getDirty() const override { return lcd.getDirty(); }

    /**
     * @brief   Check if the display contains a message for each track 
     *          separately.
     * 
     * On the original Mackie Control surfaces, the LCD display consists of two
     * 56-character lines, where each of the 8 channels has 7 characters.
     * If the LCD is used to display a message for each channel separately, the
     * seventh character of each channel is always a space, as separation 
     * between the channels.
     * 
     * @retval  true 
     *          The display contains a message for each track separately, and
     *          the messages are separated by spaces.
     * @retval  false
     *          The display contains a message that spans across multiple 
     *          tracks, without separating spaces between the tracks.
     */
    bool separateTracks() const {
        for (uint8_t i = 0; i < 7; ++i) {
            const char *text = lcd.getText() + 7 * i + 56 * line;
            if (text[6] != ' ')
                return false;
        }
        return true;
    }

    /// Set the line number of the LCD to display.
    /// @param  line
    ///         Either 1 or 2.
    void setLine(uint8_t line) { this->line = line - 1; }

  private:
    LCD<> &lcd;
    const OutputBank *bank = nullptr;
    uint8_t track;
    uint8_t line;
    int16_t x, y;
    uint8_t size;
    uint16_t color;
};

} // namespace MCU

END_CS_NAMESPACE