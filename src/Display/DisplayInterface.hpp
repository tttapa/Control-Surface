#pragma once

#include <Helpers/LinkedList.h>
#include <Print.h>

class DisplayInterface : public Print {
  protected:
    DisplayInterface() { LinkedList::append(this, first, last); }

  public:
    virtual ~DisplayInterface() { LinkedList::remove(this, first, last); }

    virtual void begin() {
        clear();
        drawBackground();
        display();
    };

    virtual void clear() = 0;
    virtual void drawBackground(){};
    virtual void display() = 0;

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    virtual void setTextColor(uint16_t color) = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual void setCursor(int16_t x, int16_t y) = 0;

    virtual size_t write(uint8_t c) override = 0;

    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          uint16_t color) = 0;
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h,
                               uint16_t color) = 0;
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w,
                               uint16_t color) = 0;
    
    virtual void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                             int16_t w, int16_t h, uint16_t color) = 0;

    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                          uint16_t color) {
        for (int16_t r = y; r < y + h; r++)
            drawFastHLine(x, r, w, color);
    }

    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
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

    virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
        int x = r;
        int y = 0;
        int err = 0;

        while (x >= y) {
            drawFastHLine(x0 - x, y0 + y, 2 * x, color);
            drawFastHLine(x0 - y, y0 + x, 2 * y, color);
            drawFastHLine(x0 - y, y0 - x, 2 * y, color);
            drawFastHLine(x0 - x, y0 - y, 2 * x, color);

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

    static void beginAll() {
        for (DisplayInterface *el = first; el; el = el->next)
            el->begin();
    }

    static void clearAll() {
        for (DisplayInterface *el = first; el; el = el->next)
            el->clear();
    }

    static void drawAllBackgrounds() {
        for (DisplayInterface *el = first; el; el = el->next)
            el->drawBackground();
    }

    static void displayAll() {
        for (DisplayInterface *el = first; el; el = el->next)
            el->display();
    }

  private:
    DisplayInterface *next;
    DisplayInterface *previous;

    static DisplayInterface *first;
    static DisplayInterface *last;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};