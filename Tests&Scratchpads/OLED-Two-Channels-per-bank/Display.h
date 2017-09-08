#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MIDI_Controller.h>

Adafruit_SSD1306 display;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#if (F_CPU != 96000000)
//#error("Set processor speed to 96 MHz!");
#endif

void drawGoodCircle(Adafruit_SSD1306 &display, int16_t x0, int16_t y0, int radius, uint8_t color = WHITE) { // draws the outline of a circle
  // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
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

void fillGoodCircle(Adafruit_SSD1306 &display, int16_t x0, int16_t y0, int radius, uint8_t color = WHITE) { // draws a solid, filled circle
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    display.drawLine(x0 + x, y0 + y, x0 - x, y0 + y, color);
    display.drawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
    display.drawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    display.drawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

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

void drawVPotSegment(Adafruit_SSD1306 &display, MCU_VPot_Ring &vpot, uint8_t x, uint8_t y, uint8_t radius, uint8_t color = WHITE) {
  if (vpot.getCenterLED())
    fillGoodCircle(display, x, y, radius / 4);
  else
    drawGoodCircle(display, x, y, radius / 4);

  uint8_t value = vpot.getPosition();
  
  if (value == 0)
    return;

  float angle = 0.4887 * (value - 6);
  uint8_t x_start = x + round((float)radius * sin(angle) / 2.0);
  uint8_t y_start = y - round((float)radius * cos(angle) / 2.0);

  uint8_t x_end = x + round((float)radius * sin(angle));
  uint8_t y_end = y - round((float)radius * cos(angle));

  display.drawLine(x_start, y_start, x_end, y_end, color);
}



void drawVU(Adafruit_SSD1306 &display, MCU_VU& vu, uint8_t x, uint8_t y, uint8_t width, uint8_t blockheight = 2, uint8_t spacing = 1, uint8_t color = WHITE) {
  uint8_t value = vu.getValue();
  for (uint8_t i = 0; i < value; i++) {
    display.fillRect(x, y - i * (blockheight + spacing), width, blockheight, color);
  }
}

struct VUPeak {
  VUPeak(MCU_VU &VU) : VU(VU) {}
  MCU_VU& VU;
  uint8_t peak = 0;
  unsigned long previousDecay = 0;
};

void drawVUPeak(Adafruit_SSD1306 &display, VUPeak& vu, uint8_t x, uint8_t y, uint8_t width, uint8_t blockheight = 2, uint8_t spacing = 1, uint8_t color = WHITE, unsigned int decayTime = 350) {
  uint8_t value = vu.VU.getValue();
  if (value >= vu.peak) {
    vu.peak = value;
    vu.previousDecay = millis();
  }
  else if (millis() - vu.previousDecay > decayTime) {
    if (vu.peak > 0) {
      vu.peak--;
      vu.previousDecay = millis();
    }
  }
  if (vu.peak > 0) {
    display.drawLine(x, y - spacing + blockheight - vu.peak * (blockheight + spacing), x + width - 1, y - spacing + blockheight - vu.peak * (blockheight + spacing), color);
    drawVU(display, vu.VU, x, y, width, blockheight, spacing, color);
  }
}

void drawTimeDisplay(Adafruit_SSD1306 &display, MCU_TimeDisplay& tdisp, uint8_t x, uint8_t y, uint8_t color = WHITE) {
  display.setTextSize(1);
  display.setTextColor(color);
  display.setCursor(x, y);
  char barStr[5], beatStr[3], frameStr[4];
  tdisp.getBars(barStr);
  tdisp.getBeats(beatStr);
  tdisp.getFrames(frameStr);
  display.print(barStr);
  display.print(' ');
  display.print(beatStr);
  display.print(' ');
  display.print(frameStr);
}

void drawCharacter(Adafruit_SSD1306 &display, MIDI_Input_Note_Buffer &nb, char character, uint8_t size, uint8_t x, uint8_t y, uint8_t color = WHITE) {
  if (nb.getState()) {
    display.setTextSize(size);
    display.setTextColor(color);
    display.setCursor(x, y);
    display.print(character);
  }
}

