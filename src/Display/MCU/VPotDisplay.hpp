#pragma once

#include <Display/DisplayElement.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>

namespace MCU {

class VPotDisplay : public DisplayElement {

  public:
    VPotDisplay(Adafruit_GFX &display, VPotRing_Base &vpot, Location loc,
                uint16_t radius, uint16_t innerRadius, uint16_t color)
        : DisplayElement(display), vpot(vpot), x(loc.x + radius),
          y(loc.y + radius), radius(radius), innerRadius(innerRadius),
          color(color) {}
    void draw() {
        drawGoodCircle(display, x, y, radius, color);
        if (vpot.getCenterLed())
            fillGoodCircle(display, x, y, innerRadius / 4, color);
        else
            drawGoodCircle(display, x, y, innerRadius / 4, color);
        uint8_t startOn = vpot.getStartOn();
        uint8_t startOff = vpot.getStartOff();
        for (uint8_t segment = startOn; segment < startOff; segment++)
            drawVPotSegment(segment);
    }

  private:
    VPotRing_Base &vpot;

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

} // namespace MCU

const float MCU::VPotDisplay::angleSpacing __attribute__((weak)) = 0.4887; // 28°