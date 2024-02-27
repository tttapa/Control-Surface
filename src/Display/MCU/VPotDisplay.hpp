#pragma once

#include <AH/STL/cmath>
#include <AH/STL/utility> // std::forward
#include <Display/DisplayElement.hpp>
#include <Display/DisplayInterface.hpp>
#include <MIDI_Inputs/InterfaceMIDIInputElements.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/**
 * @brief Displays the position of a MCU V-Pot.
 * @ingroup DisplayElements
 */
template <class VPot_t = Interfaces::MCU::IVPot &>
class VPotDisplay : public DisplayElement {

  public:
    VPotDisplay(DisplayInterface &display, VPot_t &&vpot, PixelLocation loc,
                uint16_t radius, uint16_t innerRadius, uint16_t color)
        : DisplayElement(display), vpot(std::forward<VPot_t>(vpot)),
          x(loc.x + radius), y(loc.y + radius), radius(radius),
          innerRadius(innerRadius), color(color) {}

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
        vpot.clearDirty();
    }

    bool getDirty() const override { return vpot.getDirty(); }

    void setAngleSpacing(float spacing) { this->angleSpacing = spacing; }
    float getAngleSpacing() const { return this->angleSpacing; }

  private:
    VPot_t vpot;

    int16_t x, y;
    uint16_t radius, innerRadius, color;

    float angleSpacing = 0.4887; // 28°

  protected:
    void drawVPotSegment(uint8_t segment) {
        // Segment 5 (i.e. the sixth segment) = 0° (i.e. 12 o'clock, the middle)
        float angle = angleSpacing * (segment - 5);

        uint16_t x_start = x + std::round((float)innerRadius * sin(angle) / 2);
        uint16_t y_start = y - std::round((float)innerRadius * cos(angle) / 2);

        uint16_t x_end = x + std::round((float)innerRadius * sin(angle));
        uint16_t y_end = y - std::round((float)innerRadius * cos(angle));

        display.drawLine(x_start, y_start, x_end, y_end, color);
    }
};

} // namespace MCU

END_CS_NAMESPACE