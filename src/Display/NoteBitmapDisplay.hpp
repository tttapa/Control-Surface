#pragma once

#include <Display/Bitmaps/XBitmaps.hpp>
#include <Display/DisplayElement.hpp>
#include <MIDI_Inputs/MIDINote.hpp>

/// A class that displays a bitmap depending on the state of a MIDINote.
class NoteBitmapDisplay : public DisplayElement {
  public:
    NoteBitmapDisplay(DisplayInterface &display, MIDINote_Base &note,
                      const XBitmap &xbm, PixelLocation loc, uint16_t color)
        : DisplayElement(display), note(note), xbm(xbm), x(loc.x), y(loc.y),
          color(color) {}

    void draw() override {
        if (note.getState())
            display.drawXBitmap(x, y, xbm.bits, xbm.width, xbm.height, color);
    }

  private:
    MIDINote_Base &note;
    const XBitmap &xbm;
    int16_t x, y;
    uint16_t color;
};
