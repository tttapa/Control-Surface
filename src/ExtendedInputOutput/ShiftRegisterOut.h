#include "../ExtendedIOElement.h"

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

    uint8_t redBit = 0;
    uint8_t greenBit = 1;
    uint8_t blueBit = 2;

  private:
    pin_t dataPin, clockPin, latchPin;
    uint8_t bitOrder, bufferLength;
};