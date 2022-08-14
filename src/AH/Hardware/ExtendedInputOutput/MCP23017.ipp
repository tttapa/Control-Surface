#include "MCP23017.hpp"

BEGIN_AH_NAMESPACE

enum MCP23017RegAddr {
    IODIRA = 0x00,
    IODIRB = 0x01,
    IPOLA = 0x02,
    IPOLB = 0x03,
    GPINTENA = 0x04,
    GPINTENB = 0x05,
    DEFVALA = 0x06,
    DEFVALB = 0x07,
    INTCONA = 0x08,
    INTCONB = 0x09,
    IOCON = 0x0A,
    GPPUA = 0x0C,
    GPPUB = 0x0D,
    INTFA = 0x0E,
    INTFB = 0x0F,
    INTCAPA = 0x10,
    INTCAPB = 0x11,
    GPIOA = 0x12,
    GPIOB = 0x13,
    OLATA = 0x14,
    OLATB = 0x15,
};

template <class WireType>
MCP23017<WireType>::MCP23017(WireType &wire, uint8_t addressOffset,
                             pin_t interruptPin)
    : wire(&wire), address(I2C_BASE_ADDRESS | addressOffset),
      interruptPin(interruptPin) {
    // Input mode by default
    bufferedPinModes.setByte(0, 0xFF);
    bufferedPinModes.setByte(1, 0xFF);
}

template <class WireType>
void MCP23017<WireType>::pinModeBuffered(pin_t pin, PinMode_t mode) {
    if (mode == INPUT) {
        pinModesDirty |= bufferedPinModes.get(pin) == 0;
        pullupsDirty |= bufferedPullups.get(pin) == 1;
        bufferedPinModes.set(pin);
        bufferedPullups.clear(pin);
    } else if (mode == OUTPUT) {
        pinModesDirty |= bufferedPinModes.get(pin) == 1;
        bufferedPinModes.clear(pin);
    } else if (mode == INPUT_PULLUP) {
        pinModesDirty |= bufferedPinModes.get(pin) == 0;
        pullupsDirty |= bufferedPullups.get(pin) == 0;
        bufferedPinModes.set(pin);
        bufferedPullups.set(pin);
    }
}

template <class WireType>
void MCP23017<WireType>::digitalWriteBuffered(pin_t pin, PinStatus_t status) {
    bool boolstate = status == HIGH;
    outputsDirty |= bufferedOutputs.get(pin) != boolstate;
    bufferedOutputs.set(pin, boolstate);
}

template <class WireType>
PinStatus_t MCP23017<WireType>::digitalReadBuffered(pin_t pin) {
    return bufferedInputs.get(pin) ? HIGH : LOW;
}

template <class WireType>
analog_t MCP23017<WireType>::analogReadBuffered(pin_t pin) {
    return bufferedInputs.get(pin) ? 1023 : 0;
}

template <class WireType>
void MCP23017<WireType>::analogWriteBuffered(pin_t pin, analog_t value) {
    digitalWriteBuffered(pin, value >= 0x80 ? HIGH : LOW);
}

template <class WireType>
void MCP23017<WireType>::begin() {
    if (interruptPin != NO_PIN)
        ExtIO::pinMode(interruptPin, INPUT_PULLUP);
    // Set the IOCON register (configuration register)
    writeI2C(IOCON, //
             0b01100100);
    //         │││││││└─ Unimplemented
    //         ││││││└── INTPOL = Active-low
    //         │││││└─── ODR    = Open-drain output (overrides the INTPOL bit)
    //         ││││└──── HAEN   = Disables the MCP23S17 address pins
    //         │││└───── DISSLW = Slew rate enabled
    //         ││└────── SEQOP  = Sequential operation disabled, address pointer does not increment
    //         │└─────── MIRROR = The INT pins are internally connected
    //         └──────── BANK   = The registers are in the same bank (addresses are sequential)
}

template <class WireType>
void MCP23017<WireType>::updateBufferedOutputs() {
    updateBufferedPinModes();
    if (!outputsDirty)
        return;
    writeI2C(GPIOA,                      //
             bufferedOutputs.getByte(0), //
             bufferedOutputs.getByte(1));
    outputsDirty = false;
}

template <class WireType>
void MCP23017<WireType>::updateBufferedInputs() {
    // Only update if at least one pin is configured as input
    if (!hasInputs())
        return;
    // Only update if a pin change interrupt happened
    if (interruptPin != NO_PIN && ExtIO::digitalRead(interruptPin) == HIGH)
        return;
    writeI2C(GPIOA);
    wire->requestFrom(address, size_t(2));
    bufferedInputs.setByte(0, wire->read());
    bufferedInputs.setByte(1, wire->read());
}

template <class WireType>
void MCP23017<WireType>::updateBufferedPinModes() {
    if (pinModesDirty) {
        writeI2C(IODIRA,                      //
                 bufferedPinModes.getByte(0), //
                 bufferedPinModes.getByte(1));
        writeI2C(GPINTENA,                    //
                 bufferedPinModes.getByte(0), //
                 bufferedPinModes.getByte(1));
        pinModesDirty = false;
    }
    if (pullupsDirty) {
        writeI2C(GPPUA,                      //
                 bufferedPullups.getByte(0), //
                 bufferedPullups.getByte(1));
        pullupsDirty = false;
    }
}

template <class WireType>
bool MCP23017<WireType>::hasInputs() const {
    return bufferedPinModes.getByte(0) != 0 || bufferedPinModes.getByte(1) != 0;
}

template <class WireType>
template <size_t N>
void MCP23017<WireType>::writeI2C(const uint8_t (&values)[N]) {
    this->wire->beginTransmission(address);
    this->wire->write(values, N);
    this->wire->endTransmission();
}

template <class WireType>
template <class... Args>
void MCP23017<WireType>::writeI2C(uint8_t addr, Args... values) {
    const uint8_t v[] = {addr, static_cast<uint8_t>(values)...};
    writeI2C(v);
}

END_AH_NAMESPACE
