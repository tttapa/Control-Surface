#include "DisplayInterface.hpp"

BEGIN_CS_NAMESPACE

void DisplayInterface::begin() {
    clear();
    drawBackground();
    display();
}

void DisplayInterface::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                uint16_t color) {
    for (int16_t r = y; r < y + h; r++)
        drawFastHLine(x, r, w, color);
}

void DisplayInterface::drawCircle(int16_t x0, int16_t y0, int16_t r,
                                  uint16_t color) {
    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 - y, y0 - x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 + x, y0 - y, color);

        if (err <= 0)
            err += 2 * ++y + 1;
        else
            err -= 2 * --x + 1;
    }
}

void DisplayInterface::fillCircle(int16_t x0, int16_t y0, int16_t r,
                                  uint16_t color) {
    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        drawFastHLine(x0 - x, y0 + y, 2 * x, color);
        drawFastHLine(x0 - y, y0 + x, 2 * y, color);
        drawFastHLine(x0 - y, y0 - x, 2 * y, color);
        drawFastHLine(x0 - x, y0 - y, 2 * x, color);

        if (err <= 0)
            err += 2 * ++y + 1;
        else
            err -= 2 * --x + 1;
    }
}

END_CS_NAMESPACE