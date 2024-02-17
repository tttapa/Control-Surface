#pragma once

#include <Adafruit_SSD1306.h>
#include <Display/DisplayInterface.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   This class creates a mapping between the Adafruit_SSD1306 display 
 *          driver and the general display interface used by the Control Surface
 *          library.
 * @ingroup DisplayElements
 */
class SSD1306_DisplayInterface : public DisplayInterface {
  protected:
    SSD1306_DisplayInterface(Adafruit_SSD1306 &display) : disp(display) {}

  public:
    /// Clear the frame buffer or clear the display.
    void clear() override { disp.clearDisplay(); }
    /// Draw a custom background.
    void drawBackground() override = 0;
    /// Write the frame buffer to the display. If your display library writes to
    /// the display directly, without a display buffer in RAM, you can leave
    /// this function empty.
    void display() override { disp.display(); }

    /// Paint a single pixel with the given color.
    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        disp.drawPixel(x, y, color);
    }

    /// Set the text color.
    void setTextColor(uint16_t color) override { disp.setTextColor(color); }
    /// Set the text size.
    void setTextSize(uint8_t size) override { disp.setTextSize(size); }
    /// Set the cursor position.
    void setCursor(int16_t x, int16_t y) override { disp.setCursor(x, y); }

    /**
     * @brief   Write a character to the display.
     * 
     * @see     setCursor
     * @see     setTextSize
     * @see     setTextColor
     */
    size_t write(uint8_t c) override { return disp.write(c); }

    /// Draw a line between two points.
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  uint16_t color) override {
        disp.drawLine(x0, y0, x1, y1, color);
    }
    /// Draw a vertical line.
    void drawFastVLine(int16_t x, int16_t y, int16_t h,
                       uint16_t color) override {
        disp.drawFastVLine(x, y, h, color);
    }
    /// Draw a horizontal line.
    void drawFastHLine(int16_t x, int16_t y, int16_t w,
                       uint16_t color) override {
        disp.drawFastHLine(x, y, w, color);
    }

    /// Draw a bitmap to the display.
    void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                     int16_t h, uint16_t color) override {
        disp.drawXBitmap(x, y, bitmap, w, h, color);
    }

  protected:
    Adafruit_SSD1306 &disp;
};

END_CS_NAMESPACE