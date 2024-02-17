#pragma once

#include <AH/STL/utility> // std::forward
#include <Display/DisplayElement.hpp>
#include <MIDI_Inputs/InterfaceMIDIInputElements.hpp>
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {
/**
 * @brief Displays a MCU level meter.
 * @ingroup DisplayElements
 */
template <class VU_t = Interfaces::MCU::IVU &>
class VUDisplay : public DisplayElement {
  public:
    VUDisplay(DisplayInterface &display, VU_t &&vu, PixelLocation loc,
              uint16_t width, uint8_t blockheight, uint8_t spacing,
              uint16_t color)
        : DisplayElement(display), vu(std::forward<VU_t>(vu)), x(loc.x),
          y(loc.y - blockheight + 1), width(width), blockheight(blockheight),
          spacing(spacing), color(color),
          decayTime(VU_PEAK_SMOOTH_DECAY
                        ? VU_PEAK_DECAY_TIME / (blockheight + spacing)
                        : VU_PEAK_DECAY_TIME) {}

    void draw() override {
        uint8_t value = vu.getValue();
        updatePeak(value);
        if (peak > 0) {
            drawPeak(peak);
            drawBlocks(value);
        }
        vu.clearDirty();
    }

    bool getDirty() const override {
        return vu.getDirty() || shouldStartDecaying() || shouldUpdateDecay();
    }

  protected:
    virtual void drawPeak(uint8_t peak) {
        display.drawFastHLine(x,                                //
                              y - spacing + blockheight - peak, //
                              width,                            //
                              color);
    }

    virtual void drawBlocks(uint8_t value) {
        for (uint8_t i = 0; i < value; i++)
            display.fillRect(x,                               //
                             y - i * (blockheight + spacing), //
                             width,                           //
                             blockheight,                     //
                             color);
    }

  private:
    void updatePeak(uint8_t value) {
        int16_t newPeak = (int16_t)value * (blockheight + spacing);
        if (newPeak >= peak) {
            peak = newPeak;
            previousDecay = millis();
            decaying = false;
        } else if (shouldStartDecaying()) {
            decaying = true;
            previousDecay += VU_PEAK_HOLD_TIME - decayTime;
        } else if (shouldUpdateDecay()) {
            peak -= VU_PEAK_SMOOTH_DECAY ? 1 : (blockheight + spacing);
            previousDecay += decayTime;
        }
    }

    bool shouldStartDecaying() const {
        return !decaying && peak > 0 &&
               (millis() - previousDecay > VU_PEAK_HOLD_TIME);
    }

    bool shouldUpdateDecay() const {
        return decaying && peak > 0 && //
               (millis() - previousDecay > decayTime);
    }

    VU_t vu;

    int16_t x;
    int16_t y;
    uint16_t width;
    uint8_t blockheight;
    uint8_t spacing;
    uint16_t color;

    int16_t peak = 0;
    unsigned long previousDecay = 0;
    bool decaying = false;

    unsigned long decayTime;
};

} // namespace MCU

END_CS_NAMESPACE

#include <Display/Helpers/Bresenham.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

template <class VU_t = Interfaces::MCU::IVU>
class AnalogVUDisplay : public DisplayElement {
  public:
    AnalogVUDisplay(DisplayInterface &display, VU_t &vu, PixelLocation loc,
                    uint16_t radius, float theta_min, float theta_diff,
                    uint16_t color)
        : DisplayElement(display), vu(vu), x(loc.x), y(loc.y),
          r_sq(radius * radius), theta_min(theta_min), theta_diff(theta_diff),
          color(color) {}

    void draw() override {
        float value = vu.getFloatValue();
        drawNeedle(theta_min + value * theta_diff);
        vu.clearDirty();
    }

    void drawNeedle(float angle) {
        BresenhamLine line = {{x, y}, angle};
        BresenhamLine::Pixel p = line.next();
        while (p.distanceSquared({x, y}) <= r_sq) {
            display.drawPixel(p.x, p.y, color);
            p = line.next();
        }
    }

    bool getDirty() const override { return vu.getDirty(); }

  private:
    VU_t &vu;

    int16_t x;
    int16_t y;
    uint16_t r_sq;
    float theta_min;
    float theta_diff;
    uint16_t color;
};

} // namespace MCU

END_CS_NAMESPACE
