#pragma once

#include <Def/Def.hpp>
#include <Helpers/LinkedList.hpp>
#include <Print.h>

class DisplayInterface : public Print, public DoublyLinkable<DisplayInterface> {
  protected:
    DisplayInterface() { elements.append(this); }

  public:
    virtual ~DisplayInterface() { elements.remove(this); }

    virtual void begin();

    virtual void clear() = 0;
    virtual void drawBackground(){};
    virtual void display() = 0;

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    virtual void setTextColor(uint16_t color) = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual void setCursor(int16_t x, int16_t y) = 0;

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