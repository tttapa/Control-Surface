#pragma once

#include <Banks/Bank.hpp>
#include <Display/DisplayElement.hpp>
#include <Selectors/Selector.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for displaying the setting of a Selector object.
 */
class SelectorDisplay : public DisplayElement {
  public:
    SelectorDisplay(DisplayInterface &display, SelectorBase &selector,
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
    SelectorBase &selector;
    int16_t offset, multiplier, x, y;
    uint8_t size;
    uint16_t color;
};

/**
 * @brief   A class for displaying the setting of a Bank object.
 */
class BankDisplay : public DisplayElement {
  public:
    BankDisplay(DisplayInterface &display, OutputBank &bank, int16_t offset,
                PixelLocation loc, uint8_t size, uint16_t color)
        : DisplayElement(display), bank(bank), offset(offset), x(loc.x),
          y(loc.y), size(size), color(color) {}

    void draw() override {
        display.setTextColor(color);
        display.setTextSize(size);
        display.setCursor(x, y);
        display.print(bank.getOffset() + offset);
    }

  private:
    OutputBank &bank;
    int16_t offset, x, y;
    uint8_t size;
    uint16_t color;
};

END_CS_NAMESPACE