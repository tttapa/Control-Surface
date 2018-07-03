#pragma once

#include "../MIDI_Inputs/MIDI_Input_LED.h"
#include "./Bitmaps/XBitmaps.h"
#include "./DisplayElement.hpp"

class NoteDisplay : public DisplayElement {
public:
  NoteDisplay(Adafruit_GFX &display, MIDI_Input_Note_Buffer &notebuffer,
              const XBitmap &xbm, Location loc, uint16_t color)
      : DisplayElement(display), notebuffer(notebuffer), xbm(xbm), x(loc.x),
        y(loc.y), color(color) {}

  void draw() {
    if (notebuffer.getState())
      display.drawXBitmap(x, y, xbm.bits, xbm.width, xbm.height, color);
  }

private:
  MIDI_Input_Note_Buffer &notebuffer;
  const XBitmap &xbm;
  int16_t x, y;
  uint16_t color;
};
