#ifndef VUDISPLAY_HPP
#define VUDISPLAY_HPP

#include "../MIDI_Inputs/MIDI_Input_VU.h"
#include "./DisplayElement.hpp"

class VUDisplay : public DisplayElement
{
public:
  VUDisplay(Adafruit_GFX &display, MCU_VU &vu,
            Location loc, uint16_t width,
            uint8_t blockheight, uint8_t spacing,
            uint16_t color)
      : DisplayElement(display), vu(vu),
        x(loc.x), y(loc.y), width(width),
        blockheight(blockheight), spacing(spacing),
        color(color) {}
  void draw()
  {
    uint8_t value = vu.getValue();

    if (value >= peak)
    {
      peak = value;
      previousDecay = millis();
    }
    else if (millis() - previousDecay > decayTime)
    {
      if (peak > 0)
      {
        peak--;
        previousDecay = millis();
      }
    }
    if (peak > 0)
    {
      display
          .drawFastHLine(x,
                         y - spacing + blockheight - peak * (blockheight + spacing),
                         width,
                         color);
      for (uint8_t i = 0; i < value; i++)
        display.fillRect(x,
                         y - i * (blockheight + spacing),
                         width,
                         blockheight,
                         color);
    }
  }

private:
  MCU_VU &vu;

  int16_t x, y;
  uint16_t width;
  uint8_t blockheight, spacing;
  uint16_t color;

  unsigned long peak = 0;
  unsigned long previousDecay = 0;

  const static unsigned long decayTime;
};

const unsigned long VUDisplay::decayTime = 350;

#endif // VUDISPLAY_HPP