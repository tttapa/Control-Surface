#pragma once

#include <AH/Containers/LinkedList.hpp>
#include <Def/Def.hpp>
#include <Print.h>

BEGIN_CS_NAMESPACE

/**
 * @brief   An interface for displays. 
 * 
 * Inspired by the Adafruit GFX library for easy compatibility.
 */
class DisplayInterface : public Print {
  protected:
    DisplayInterface() = default;

  public:
    virtual ~DisplayInterface() = default;

    /// Initialize the display.
    virtual void begin();

    /// Clear the frame buffer or clear the display.
    virtual void clear() = 0;
    /// Draw a custom background.
    virtual void drawBackground(){};
    /// Write the frame buffer to the display. If your display library writes to
    /// the display directly, without a display buffer in RAM, you can leave 
    /// this function empty.
    virtual void display() = 0;

    /// Paint a single pixel with the given color.
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    /// Set the text color.
    virtual void setTextColor(uint16_t color) = 0;
    /// Set the text size.
    virtual void setTextSize(uint8_t size) = 0;
    /// Set the cursor position.
    virtual void setCursor(int16_t x, int16_t y) = 0;

    /**
     * @brief   Write a character to the display.
     * 
     * @see     setCursor
     * @see     setTextSize
     * @see     setTextColor
     */
    size_t write(uint8_t c) override = 0;

    /// Draw a line between two points.
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          uint16_t color) = 0;
    /// Draw a vertical line.
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h,
                               uint16_t color) = 0;
    /// Draw a horizontal line.
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w,
                               uint16_t color) = 0;

    /// Draw a bitmap to the display.
    virtual void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                             int16_t w, int16_t h, uint16_t color) = 0;

    /// Draw a filled rectangle.
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                          uint16_t color);

    /// Draw a circle.
    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

    /// Draw a disk (filled circle).
    virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

    /**
     * @brief   Clear the frame buffer, and draw the custom background.
     * @see    clear
     * @see    drawBackground
     */
    void clearAndDrawBackground() {
        clear();
        drawBackground();
    }
};

END_CS_NAMESPACE