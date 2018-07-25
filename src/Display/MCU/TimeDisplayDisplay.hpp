#pragma once

#include <Display/DisplayElement.hpp>
#include <Display/DisplayInterface.hpp>
#include <MIDI_Inputs/MCU/TimeDisplay.hpp>

namespace MCU {

class TimeDisplayDisplay : public DisplayElement {
  public:
    TimeDisplayDisplay(DisplayInterface &display,
                       const TimeDisplay &timedisplay, Location loc,
                       uint8_t size, uint16_t color)
        : DisplayElement(display), timedisplay(timedisplay), x(loc.x), y(loc.y),
          size(size), color(color) {}
    void draw() {
        display.setTextColor(color);
        display.setTextSize(size);
        display.setCursor(x, y);
        char barStr[5], beatStr[3], frameStr[4];
        timedisplay.getBars(barStr);
        timedisplay.getBeats(beatStr);
        timedisplay.getFrames(frameStr);
        display.print(barStr);
        display.print(' ');
        display.print(beatStr);
        display.print(' ');
        display.print(frameStr);
    }

    int16_t getX() const { return x; }
    int16_t getY() const { return y; }
    uint8_t getSize() const { return size; }
    uint16_t getColor() const { return color; }

    void setX(int16_t x) { this->x = x; }
    void setY(int16_t y) { this->y = y; }
    void setSize(uint8_t size) { this->size = size; }
    void setColor(uint16_t color) { this->color = color; }

  private:
    const TimeDisplay &timedisplay;
    int16_t x, y;
    uint8_t size;
    uint16_t color;
};

} // namespace MCU