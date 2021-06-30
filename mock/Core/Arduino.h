/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "binary.h"

#define PROGMEM
struct __FlashStringHelper;
#define pgm_read_ptr_near(ptr) ((void *) *(ptr))
#define pgm_read_byte_near(ptr) (*((const uint8_t *)(ptr)))
#define F(x) (reinterpret_cast<const __FlashStringHelper *>(x))

#define HIGH 0x1
#define LOW 0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define LED_BUILTIN 13

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define NUM_DIGITAL_PINS 14
#define NUM_ANALOG_INPUTS 6

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

typedef bool boolean;

// #define min(a,b) ((a)<(b)?(a):(b))
// #define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x) > 0 ? (x) : -(x))
#define constrain(amt, low, high)                                              \
    ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x) std::round(x)
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define interrupts() sei()
#define noInterrupts() cli()

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);
void analogReadResolution(uint8_t);
void analogWrite(uint8_t, int);

unsigned long millis(void);
unsigned long micros(void);

void delay(unsigned long);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

#include "HardwareSerial.h"

void cli();
void sei();

void yield();

#define PIN_A0   (14)
#define PIN_A1   (15)
#define PIN_A2   (16)
#define PIN_A3   (17)
#define PIN_A4   (18)
#define PIN_A5   (19)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#include "ArduinoMock.hpp"

#endif