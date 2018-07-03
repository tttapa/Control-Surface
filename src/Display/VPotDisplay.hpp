#pragma once

#include "../MIDI_Inputs/MIDI_Input_V-Pot_Ring.h"
#include "./DisplayElement.hpp"

#include <math.h>

#undef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#undef min
#define min(a, b) ((a) < (b) ? (a) : (b))

class VPotDisplay : public DisplayElement {

public:
  VPotDisplay(Adafruit_GFX &display, MCU_VPot_Ring &vpot, Location loc,
              uint16_t radius, uint16_t innerRadius, uint16_t color)
      : DisplayElement(display), vpot(vpot), x(loc.x + radius),
        y(loc.y + radius), radius(radius), innerRadius(innerRadius),
        color(color) {}
  void draw() {
    drawCircle(display, x, y, radius, color);
    if (vpot.getCenterLED())
      fillCircle(display, x, y, innerRadius / 4, color);
    else
      drawCircle(display, x, y, innerRadius / 4, color);

    uint8_t value = vpot.getPosition();
    if (value == 0)
      return;
    value--;
    
    switch (vpot.getMode()) {

    case 0: {
      drawVPotSegment(value);
    } break;

    case 1: {
      uint8_t startOn = min(value, 5);
      uint8_t startOff = max(value, 5) + 1;
      for (uint8_t pin = startOn; pin < startOff; pin++)
        drawVPotSegment(pin);
    } break;

    case 2: {
      for (uint8_t pin = 0; pin <= value; pin++)
        drawVPotSegment(pin);
    } break;

    case 3: {
      uint8_t startOn = max(5 - value, 0);
      uint8_t startOff = min(6 + value, 11);
      for (uint8_t pin = startOn; pin < startOff; pin++)
        drawVPotSegment(pin);
    } break;
    }
  }

private:
  MCU_VPot_Ring &vpot;

  int16_t x, y;
  uint16_t radius, innerRadius, color;

  const static float angleSpacing;

protected:
  void drawVPotSegment(uint8_t segment) {
    // segment 5 (i.e. the sixth segment) = 0° (i.e. 12 o'clock)
    float angle = angleSpacing * (segment - 5);

    uint16_t x_start = x + round((float)innerRadius * sin(angle) / 2.0);
    uint16_t y_start = y - round((float)innerRadius * cos(angle) / 2.0);

    uint16_t x_end = x + round((float)innerRadius * sin(angle));
    uint16_t y_end = y - round((float)innerRadius * cos(angle));

    display.drawLine(x_start, y_start, x_end, y_end, color);
  }
};

const float VPotDisplay::angleSpacing = 0.4887; // 28°