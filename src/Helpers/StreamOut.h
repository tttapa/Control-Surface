#ifndef STREAMOUT_H_
#define STREAMOUT_H_

#include "Arduino.h"
#include "is_integral.h"

enum StreamOut_format_t
{
    dec,
    hex,
    bin,
};

static StreamOut_format_t streamOutFormat = dec;

inline Print &operator<<(Print &printer, StreamOut_format_t format)
{
    streamOutFormat = format;
    return printer;
}
template <class T>
inline Print &operator<<(Print &printer, T printable)
{
    if (is_integral_type<T>())
        printInt(printer, printable);
    else
        print(printer, printable);
    return printer;
}

// For all types except integral types:
template <typename T>
void print(Print &printer, T printable)
{
    printer.print(printable);
}

// For integral types only:
template <class T>
void printInt(Print &printer, T printable)
{
    switch (streamOutFormat)
    {
    case dec:
    {
        printer.print(printable);
        break;
    }
    case hex:
    {
        printHex(printable, printer);
        break;
    }
    case bin:
    {
        printBin(printable, printer);
        break;
    }
    }
}

#if __BYTE_ORDER != __LITTLE_ENDIAN
#error "Byte order not supported"
#endif
template <class T>
void printHex(T val, Print &printer = Serial)
{
    for (int i = sizeof(val) - 1; i >= 0; i--)
    {
        printer.print(((uint8_t *)&val)[i] >> 4, HEX);
        printer.print(((uint8_t *)&val)[i] & 0xF, HEX);
        if (i)
            printer.print(' ');
    }
}

template <class T>
void printBin(T val, Print &printer = Serial)
{
    for (int i = sizeof(val) - 1; i >= 0; i--)
    {
        uint8_t currByte = ((uint8_t *)&val)[i];
        for (int j = 7; j >= 0; j--)
        {
            printer.print(currByte & (1 << j) ? '1' : '0');
        }
        if (i)
            printer.print(' ');
    }
}

extern const char *endl;
extern char tab;

#endif // STREAMOUT_H_