#pragma once

#include <Adafruit_SSD1306.h>
#include <Display/DisplayInterface.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   This class creates a mapping between the Adafruit_SSD1306 display 
 *          driver and the general display interface used by the Control Surface
 *          library.
 */
class SSD1306_DisplayInterface : public DisplayInterface {
  protected:
    SSD1306_DisplayInterface(Adafruit_SSD1306 &display) : disp(display) {}

  public:
    void clear() override { disp.clearDisplay(); }
    void drawBackground() override = 0;
    void display() override { disp.display(); }

    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        disp.drawPixel(x, y, color);
    }

    void setTextColor(uint16_t color) override { disp.setTextColor(color); }
    void setTextSize(uint8_t size) override { disp.setTextSize(size); }
    void setCursor(int16_t x, int16_t y) override { disp.setCursor(x, y); }

    size_t write(uint8_t c) override { return disp.write(c); }

    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  uint16_t color) override {
        disp.drawLine(x0, y0, x1, y1, color);
    }
    void drawFastVLine(int16_t x, int16_t y, int16_t h,
                       uint16_t color) override {
        disp.drawFastVLine(x, y, h, color);
    }
    void drawFastHLine(int16_t x, int16_t y, int16_t w,
                       uint16_t color) override {
        disp.drawFastHLine(x, y, w, color);
    }

    void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                     int16_t h, uint16_t color) override {
        disp.drawXBitmap(x, y, bitmap, w, h, color);
    }

  protected:
    Adafruit_SSD1306 &disp;
};

END_CS_NAMESPACE