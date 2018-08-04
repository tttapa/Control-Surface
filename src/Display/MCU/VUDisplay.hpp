#pragma once

#include <Display/DisplayElement.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

namespace MCU {

class VUDisplay : public DisplayElement {
  public:
    VUDisplay(DisplayInterface &display, IVU &vu, Location loc,
              uint16_t width, uint8_t blockheight, uint8_t spacing,
              uint16_t color)
        : DisplayElement(display), vu(vu), x(loc.x), y(loc.y - blockheight + 1),
          width(width), blockheight(blockheight), spacing(spacing),
          color(color),
          decayTime(VU_PEAK_SMOOTH_DECAY
                        ? VU_PEAK_DECAY_TIME / (blockheight + spacing)
                        : VU_PEAK_DECAY_TIME) {}
    void draw() {
        uint8_t value = vu.getValue();

        int16_t newPeak = (int16_t)value * (blockheight + spacing);
        if (newPeak >= peak) {
            peak = newPeak;
            previousDecay = millis();
            decaying = false;
        } else if (!decaying &&
                   (millis() - previousDecay > VU_PEAK_HOLD_TIME)) {
            decaying = true;
            previousDecay += VU_PEAK_HOLD_TIME - VU_PEAK_DECAY_TIME;
        } else if (decaying && (millis() - previousDecay > decayTime)) {
            if (peak > 0) {
                peak -= VU_PEAK_SMOOTH_DECAY ? 1 : (blockheight + spacing);
                previousDecay += decayTime;
            }
        }
        if (peak > 0) {
            display.drawFastHLine(x, y - spacing + blockheight - peak, width,
                                  color);
            for (uint8_t i = 0; i < value; i++)
                display.fillRect(x, y - i * (blockheight + spacing), width,
                                 blockheight, color);
        }
    }

  private:
    IVU &vu;

    int16_t x;
    int16_t y;
    uint16_t width;
    uint8_t blockheight;
    uint8_t spacing;
    uint16_t color;

    int16_t peak = 0;
    unsigned long previousDecay = 0;
    bool decaying = false;

    unsigned long decayTime;
};

} // namespace MCU