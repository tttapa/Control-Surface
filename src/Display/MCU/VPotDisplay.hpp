#pragma once

#include <Display/DisplayElement.hpp>
#include <Display/DisplayInterface.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>
#include <math.h>

BEGIN_CS_NAMESPACE

namespace MCU {

class VPotDisplay : public DisplayElement {

  public:
    VPotDisplay(DisplayInterface &display, IVPotRing &vpot, PixelLocation loc,
                uint16_t radius, uint16_t innerRadius, uint16_t color)
        : DisplayElement(display), vpot(vpot), x(loc.x + radius),
          y(loc.y + radius), radius(radius), innerRadius(innerRadius),
          color(color) {}
    void draw() override {
        display.drawCircle(x, y, radius, color);
        if (vpot.getCenterLed())
            display.fillCircle(x, y, innerRadius / 4, color);
        else
            display.drawCircle(x, y, innerRadius / 4, color);
        uint8_t startOn = vpot.getStartOn();
        uint8_t startOff = vpot.getStartOff();
        for (uint8_t segment = startOn; segment < startOff; segment++)
            drawVPotSegment(segment);
    }

  private:
    IVPotRing &vpot;

    int16_t x, y;
    uint16_t radius, innerRadius, color;

    const static float angleSpacing;

  protected:
    void drawVPotSegment(uint8_t segment) {
        // segment 5 (i.e. the sixth segment) = 0° (i.e. 12 o'clock)
        float angle = angleSpacing * (segment - 5);

        // TODO: use Bresenham directly

        uint16_t x_start = x + round((float)innerRadius * sin(angle) / 2);
        uint16_t y_start = y - round((float)innerRadius * cos(angle) / 2);

        uint16_t x_end = x + round((float)innerRadius * sin(angle));
        uint16_t y_end = y - round((float)innerRadius * cos(angle));

        display.drawLine(x_start, y_start, x_end, y_end, color);
    }
};

} // namespace MCU

END_CS_NAMESPACE