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

class HardwareSerial : public Stream {
  public:
    MOCK_METHOD1(begin, void(unsigned long));
    MOCK_METHOD0(end, void());

    MOCK_METHOD0(available, int());
    MOCK_METHOD0(read, int());
    MOCK_METHOD0(peek, int());

    MOCK_METHOD1(write, size_t(uint8_t));
    MOCK_METHOD2(write, size_t(const uint8_t *, size_t));
};

extern HardwareSerial Serial;

#ifndef F
#define F(x) (x)
#endif

#endif
