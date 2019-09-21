#pragma once

#include <Display/DisplayElement.hpp>
#include <Selectors/Selector.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for displaying the setting of a Selector object.
 * 
 * @tparam  N 
 *          The number of settings.
 * @todo    Super class to Selector to remove template?
 */
template <setting_t N>
class SelectorDisplay : public DisplayElement {
  public:
    SelectorDisplay(DisplayInterface &display, Selector<N> &selector,
                    int16_t offset, int16_t multiplier, PixelLocation loc,
                    uint8_t size, uint16_t color)
        : DisplayElement(display), selector(selector), offset(offset),
          multiplier(multiplier), x(loc.x), y(loc.y), size(size), color(color) {
    }
    void draw() override {
        display.setTextColor(color);
        display.setTextSize(size);
        display.setCursor(x, y);
        display.print(selector.get() * multiplier + offset);
    }

  private:
    Selector<N> &selector;
    int16_t offset, multiplier, x, y;
    uint8_t size;
    uint16_t color;
};

END_CS_NAMESPACE