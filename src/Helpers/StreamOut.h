#ifndef STREAMOUT_H
#define STREAMOUT_H

#ifdef ARDUINO

#include <Arduino.h>
#include "is_integral.h"

enum StreamOut_format_t
{
    dec = 10,
    hex = 16, 
    bin = 2,
};
enum StreamOut_leadingZeros_t
{
    noLeadingZeros = 0,
    leadingZeros = 1,
};

extern StreamOut_format_t streamOutFormat;
extern StreamOut_leadingZeros_t streamOutLeadingZeros;
inline Print &operator<<(Print &printer, StreamOut_format_t format)
{
    streamOutFormat = format;
    return printer;
}
inline Print &operator<<(Print &printer, StreamOut_leadingZeros_t zeros)
{
    streamOutLeadingZeros = zeros;
    return printer;
}
template <class T>
inline Print &operator<<(Print &printer, const T printable)
{
    if (is_integral_type<T>())  {
        printInt(printer, printable);
        // printer.print("<int:");
        // printer.print(streamOutFormat);
        // printer.print("> ");
    }
    else 
    {
        print(printer, printable);
        // printer.print("<non-int> ");
    }

    return printer;
}

// For all types except integral types:
template <typename T>
void print(Print &printer, const T printable)
{
    printer.print(printable);
}

// For integral types only:
template <class T>
void printInt(Print &printer, const T printable)
{
    switch (streamOutFormat)
    {
    case dec:
        printer.print(printable);
        break;
    case hex:
        printHex(printable, printer);
        break;
    case bin:
        printBin(printable, printer);  
        break;
    }
}

static char nibble_to_hex(uint8_t nibble) {  // convert a 4-bit nibble to a hexadecimal character
  nibble &= 0xF;
  return nibble > 9 ? nibble - 10 + 'A' : nibble + '0';
}

#if __BYTE_ORDER != __LITTLE_ENDIAN
#error "Byte order not supported"
#endif
template <class T>
void printHex(T val, Print &printer = Serial)
{
    bool nonZero = false;
    for (int i = sizeof(val) - 1; i >= 0; i--)
    {
        uint8_t currByte = ((uint8_t *)&val)[i];
        if (currByte != 0 || i == 0)
            nonZero = true;
        if (streamOutLeadingZeros == leadingZeros || nonZero) {
            printer.print(nibble_to_hex(currByte >> 4));
            printer.print(nibble_to_hex(currByte));
            if (i)
                printer.print(' ');
        }
    }
}

template <class T>
void printBin(T val, Print &printer = Serial)
{
    bool nonZero = false;
    for (int i = sizeof(val) - 1; i >= 0; i--)
    {
        uint8_t currByte = ((uint8_t *)&val)[i]; 
        if (currByte != 0 || i == 0)
            nonZero = true;
        if (streamOutLeadingZeros == leadingZeros || nonZero) {
            for (int j = 7; j >= 0; j--)
            {
                printer.print(currByte & (1 << j) ? '1' : '0');
            }
            if (i)
                printer.print(' ');
        }
    }
}

extern const char *endl;

#else // Tests without Arduino

#include <iostream>
using namespace std;

#endif

extern char tab;

#endif // STREAMOUT_H
