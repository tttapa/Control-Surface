#pragma once

#include <Display/DisplayElement.hpp>
#include <MIDI_Inputs/MCU/TimeDisplay.hpp>

namespace MCU {

class TimeDisplayDisplay : public DisplayElement {
  public:
    TimeDisplayDisplay(Adafruit_GFX &display, TimeDisplay &tdisp, Location loc,
                       uint8_t size, uint16_t color)
        : DisplayElement(display), tdisp(tdisp), x(loc.x), y(loc.y), size(size),
          color(color) {}
    void draw() {
        display.setTextColor(color);
        display.setTextSize(size);
        display.setCursor(x, y);
        char barStr[5], beatStr[3], frameStr[4];
        tdisp.getBars(barStr);
        tdisp.getBeats(beatStr);
        tdisp.getFrames(frameStr);
        display.print(barStr);
        display.print(' ');
        display.print(beatStr);
        display.print(' ');
        display.print(frameStr);
    }

  private:
    TimeDisplay &tdisp;
    int16_t x, y;
    uint8_t size;
    uint16_t color;
};

} // namespace MCU