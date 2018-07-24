#pragma once

#include <Display/Bitmaps/XBitmaps.h>
#include <Display/DisplayElement.hpp>
#include <MIDI_Inputs/MIDINote.hpp>

class NoteDisplay : public DisplayElement {
  public:
    NoteDisplay(Adafruit_GFX &display, MIDINote_Base &note, const XBitmap &xbm,
                Location loc, uint16_t color)
        : DisplayElement(display), note(note), xbm(xbm), x(loc.x), y(loc.y),
          color(color) {}

    void draw() {
        if (note.getState())
            display.drawXBitmap(x, y, xbm.bits, xbm.width, xbm.height, color);
    }

  private:
    MIDINote_Base &note;
    const XBitmap &xbm;
    int16_t x, y;
    uint16_t color;
};
