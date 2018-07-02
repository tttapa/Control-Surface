#ifndef DISPLAYHELPERS_HPP
#define DISPLAYHELPERS_HPP

#include <Adafruit_GFX.h>

/**
 * @brief Draws the outline of a circle
 * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
void drawCircle(Adafruit_GFX &display, int16_t x0, int16_t y0, uint16_t radius,
                uint16_t color) {
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    display.drawPixel(x0 + x, y0 + y, color);
    display.drawPixel(x0 + y, y0 + x, color);
    display.drawPixel(x0 - y, y0 + x, color);
    display.drawPixel(x0 - x, y0 + y, color);
    display.drawPixel(x0 - x, y0 - y, color);
    display.drawPixel(x0 - y, y0 - x, color);
    display.drawPixel(x0 + y, y0 - x, color);
    display.drawPixel(x0 + x, y0 - y, color);

    if (err <= 0) {
      y++;
      err += 2 * y + 1;
    }
    if (err > 0) {
      x--;
      err -= 2 * x + 1;
    }
  }
}

/**
 * @brief Fills a circle
 */
void fillCircle(Adafruit_GFX &display, int16_t x0, int16_t y0, uint16_t radius,
                uint16_t color) {
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    display.drawFastHLine(x0 - x, y0 + y, 2 * x, color);
    display.drawFastHLine(x0 - y, y0 + x, 2 * y, color);
    display.drawFastHLine(x0 - y, y0 - x, 2 * y, color);
    display.drawFastHLine(x0 - x, y0 - y, 2 * x, color);

    if (err <= 0) {
      y++;
      err += 2 * y + 1;
    }
    if (err > 0) {
      x--;
      err -= 2 * x + 1;
    }
  }
}

#endif // DISPLAYHELPERS_HPP