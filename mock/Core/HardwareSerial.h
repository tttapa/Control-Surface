/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include "Stream.h"
#include <inttypes.h>

#ifdef F
#undef F
#endif

#include <gmock/gmock.h>

class SerialHelper {
  public:
    MOCK_METHOD(void, begin, (unsigned long));
    MOCK_METHOD(void, end, ());

    MOCK_METHOD(int, available, ());
    MOCK_METHOD(int, read, ());
    MOCK_METHOD(int, peek, ());

    MOCK_METHOD(size_t, write, (uint8_t));
    MOCK_METHOD(size_t, write, (const uint8_t *, size_t));

    virtual ~SerialHelper() = default;
};

class HardwareSerial : public Stream {
  public:
    void begin(unsigned long baud);
    void end();

    int available() override;
    int read() override;
    int peek() override;

    size_t write(uint8_t data) override;
    size_t write(const uint8_t *data, size_t len) override;
    explicit operator bool() const { return true; }
};

extern HardwareSerial Serial;

#ifndef F
#define F(x) (reinterpret_cast<const __FlashStringHelper *>(x))
#endif

#endif
