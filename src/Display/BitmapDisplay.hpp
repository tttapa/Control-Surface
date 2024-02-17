#pragma once

#include <AH/STL/utility> // std::forward
#include <Display/Bitmaps/XBitmaps.hpp>
#include <Display/DisplayElement.hpp>
#include <MIDI_Inputs/InterfaceMIDIInputElements.hpp>

BEGIN_CS_NAMESPACE

/// A class that displays a bitmap depending on the state of a MIDINote or any
/// other object that has a `getValue()` method that returns true (or anything
/// that evaluates to true) when the bitmap has to be displayed, as wel as a
/// pair of `getDirty()`/`clearDirty()` methods to determine whether the display
/// has to be updated.
/// @ingroup DisplayElements
template <class Value_t = Interfaces::IValue &>
class BitmapDisplay : public DisplayElement {
  public:
    BitmapDisplay(DisplayInterface &display, Value_t &&value,
                  const XBitmap &xbm, PixelLocation loc, uint16_t color)
        : DisplayElement(display), value(std::forward<Value_t>(value)),
          xbm(xbm), x(loc.x), y(loc.y), color(color) {}

    void draw() override {
        if (value.getValue())
            display.drawXBitmap(x, y, xbm.bits, xbm.width, xbm.height, color);
        value.clearDirty();
    }

    bool getDirty() const override { return value.getDirty(); }

  private:
    Value_t value;
    const XBitmap &xbm;
    int16_t x, y;
    uint16_t color;
};

template <class Value_t = Interfaces::IValue>
using NoteBitmapDisplay [[deprecated("Use BitmapDisplay instead")]] =
    BitmapDisplay<Value_t>;

END_CS_NAMESPACE