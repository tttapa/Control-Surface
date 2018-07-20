#pragma once

#include "../MIDI_Inputs/MIDI_Input_SevenSegmentDisplay.h"
#include "./DisplayElement.hpp"

class TimeDisplay : public DisplayElement {
public:
  TimeDisplay(Adafruit_GFX &display, MCU_TimeDisplay &tdisp, Location loc,
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
  MCU_TimeDisplay &tdisp;
  int16_t x, y;
  uint8_t size;
  uint16_t color;
};
