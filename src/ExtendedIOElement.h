#ifndef EXTENDEDIOELEMENT_H_
#define EXTENDEDIOELEMENT_H_

#include "Arduino.h"
// #include "ExtendedInputOutput.h"
// using namespace ExtIO;

// #define DEBUG

using pin_t = unsigned int;
using analog_t = int;

class ExtendedIOElement
{
  public:
    ExtendedIOElement(){};
    ~ExtendedIOElement(){};

    virtual void pinMode(pin_t pin, uint8_t mode){};
    virtual void digitalWrite(pin_t pin, uint8_t mode){};
    virtual int digitalRead(pin_t pin){};
    virtual analog_t analogRead(pin_t pin){};
    virtual void analogWrite(pin_t pin, analog_t val){};

    virtual void begin(){};
    virtual void refresh(){};
    virtual void reset(){};

    pin_t pin(pin_t p)
    {
        return p + offset;
    }

    void setOffset(pin_t offset)
    {
        this->offset = offset;
    }

    pin_t getLastPin()
    {
        return offset + length;
    }

    pin_t getLength()
    {
        return length;
    }

  protected:
    pin_t length;
    uint8_t *stateBuffer = nullptr;
    analog_t *analogBuffer = nullptr;

    pin_t offset;

    int8_t pinToBufferIndex(pin_t pin)
    {
        if (pin >= length)
            return INVALID_PIN;
        return pin >> 3; // pin / 8;
    }
    uint8_t pinToBitMask(pin_t pin)
    {
        return 1 << (pin & 7); // 1 << (pin % 8);
    }
    const int8_t INVALID_PIN = -1;
};

class ShiftRegisterOut : public ExtendedIOElement
{
  public:
    ShiftRegisterOut(pin_t dataPin, pin_t clockPin, pin_t latchPin, uint8_t bitOrder, pin_t length = 8)
        : dataPin(dataPin), clockPin(clockPin), latchPin(latchPin), bitOrder(bitOrder)
    {
        this->length = length;
        bufferLength = (length + 7) / 8;
        stateBuffer = (uint8_t *)malloc(bufferLength);
    }
    ~ShiftRegisterOut()
    {
        free(stateBuffer);
    }

    void digitalWrite(pin_t pin, uint8_t val)
    {
#ifdef DEBUG
        Serial.print("digitalWrite(");
        Serial.print(pin);
        Serial.print(", ");
        Serial.print(val);
        Serial.print(");\r\n");
#endif

        int8_t bufferIndex = pinToBufferIndex(pin);
#ifdef DEBUG_F
        Serial.print("bufferIndex: ");
        Serial.println(bufferIndex);
        Serial.print("pinToBitMask: ");
        Serial.println(pinToBitMask(pin));
#endif

        if (bufferIndex == INVALID_PIN)
            return;
        if (val)
            stateBuffer[bufferIndex] |= pinToBitMask(pin);
        else
            stateBuffer[bufferIndex] &= ~pinToBitMask(pin);
        refresh();
    }

    void pinMode(pin_t pin, uint8_t mode)
    {
        //#ifdef DEBUG
        Serial.print("ShiftRegPinMode(");
        Serial.print(pin);
        Serial.print(", ");
        Serial.print(mode);
        Serial.print(");\r\n");
        //#endif
    }

    void begin()
    {
#ifdef DEBUG
        Serial.println("ShiftOut\tBegin");
#endif
        ::pinMode(dataPin, OUTPUT);
        ::pinMode(clockPin, OUTPUT);
        ::pinMode(latchPin, OUTPUT);
        reset();
#ifdef DEBUG_F
        Serial.print("length: ");
        Serial.println(length);
        Serial.print("bufferLength: ");
        Serial.println(bufferLength);
#endif
    }

    void refresh()
    {
#ifdef DEBUG
        Serial.println("ShiftOut\tRefresh");
#endif
        ::digitalWrite(latchPin, LOW);
        if (bitOrder == LSBFIRST)
            for (uint8_t i = 0; i < bufferLength; i++)
                shiftOut(dataPin, clockPin, LSBFIRST, stateBuffer[i]);
        else
            for (int8_t i = bufferLength - 1; i >= 0; i--)
                shiftOut(dataPin, clockPin, MSBFIRST, stateBuffer[i]);
        ::digitalWrite(latchPin, HIGH);
    }

    void reset()
    {
#ifdef DEBUG
        Serial.println("ShiftOut\tReset");
#endif
        memset(stateBuffer, 0, bufferLength);
        refresh();
    }

    pin_t green(pin_t id)
    {
        return 3 * id + greenBit + offset;
    }
    pin_t red(pin_t id)
    {
        return 3 * id + redBit + offset;
    }
    pin_t blue(pin_t id)
    {
        return 3 * id + blueBit + offset;
    }

  private:
    pin_t dataPin, clockPin, latchPin;
    uint8_t bitOrder, bufferLength;
};

#endif