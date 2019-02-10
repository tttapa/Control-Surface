#pragma once

#include <Def/Def.hpp>
#include <Helpers/LinkedList.hpp>
#include <Print.h>

/**
 * @brief   An interface for displays. 
 * 
 * Inspired by the Adafruit GFX library for easy compatibility.
 */
class DisplayInterface : public Print, public DoublyLinkable<DisplayInterface> {
  protected:
    /// @todo   Do I need to keep a list now that I have sorted all 
    ///         DisplayElement#s?
    DisplayInterface() { elements.append(this); }

  public:
    /// @todo   Do I need to keep a list now that I have sorted all 
    ///         DisplayElement#s?
    // Note to self:    don't forget to make destructor = default 
    //                  instead of deleting it altogether
    virtual ~DisplayInterface() { elements.remove(this); }

    /// Initialize the display.
    virtual void begin();

    /// Clear the frame buffer or display.
    virtual void clear() = 0;
    /// Draw a custom background.
    virtual void drawBackground(){};
    /// Write the frame buffer to the display.
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

    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          uint16_t color) = 0;
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h,
                               uint16_t color) = 0;
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w,
                               uint16_t color) = 0;

    virtual void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                             int16_t w, int16_t h, uint16_t color) = 0;

    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                          uint16_t color);

    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

    virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

    static void beginAll();

    void clearAndDrawBackground() {
        clear();
        drawBackground();
    }

  private:
    static DoublyLinkedList<DisplayInterface> elements;
};