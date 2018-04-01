#ifndef SELECTORDISPLAY_HPP
#define SELECTORDISPLAY_HPP

#include "../Selectors/Selector.h"
#include "./DisplayElement.hpp"

class SelectorDisplay : public DisplayElement
{
public:
  SelectorDisplay(Adafruit_GFX &display, Selector &selector,
                  int16_t offset, int16_t multiplier,
                  Location loc,
                  uint8_t size, uint16_t color)
      : DisplayElement(display), selector(selector),
        offset(offset), multiplier(multiplier),
        x(loc.x), y(loc.y),
        size(size), color(color) {}
  void draw()
  {
    display.setTextColor(color);
    display.setTextSize(size);
    display.setCursor(x, y);
    display.print(selector.getSetting() * multiplier + offset);
  }

private:
  Selector &selector;
  int16_t offset, multiplier, x, y;
  uint8_t size;
  uint16_t color;
};

#endif // SELECTORDISPLAY_HPP