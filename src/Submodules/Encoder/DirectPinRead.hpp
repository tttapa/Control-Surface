#pragma once

#include <AH/Arduino-Wrapper.h>
#include <Settings/NamespaceSettings.hpp>

#include <AH/STL/type_traits>
#include <Def/Def.hpp>

#ifdef ARDUINO_ARCH_NRF52840
#include <pins_arduino.h>
#endif

BEGIN_CS_NAMESPACE

template <class RegisterType = const volatile uint32_t>
struct DirectPinReadReg {
    RegisterType *in_reg;
    typename std::remove_cv<RegisterType>::type bitmask;
    bool read() const { return *in_reg & bitmask; }
};

// 8-bit AVR
#if defined(__AVR__) ||                                                        \
    (defined(TEENSYDUINO) && (defined(KINETISK) || defined(KINETISL)))

using DirectPinRead = DirectPinReadReg<const volatile uint8_t>;
inline DirectPinRead direct_pin_read(pin_t pin) {
    return {portInputRegister(digitalPinToPort(pin)),
            static_cast<uint8_t>(digitalPinToBitMask(pin))};
}

// 32-bit
#elif defined(__SAM3X8E__) || defined(ESP32) || defined(ESP8266) ||            \
    defined(__SAMD51__)

using DirectPinRead = DirectPinReadReg<const volatile uint32_t>;
inline DirectPinRead direct_pin_read(pin_t pin) {
    return {portInputRegister(digitalPinToPort(pin)), digitalPinToBitMask(pin)};
}

// Special cases
#elif defined(__SAMD21G18A__) || defined(__SAMD21E18A__)

using DirectPinRead = DirectPinReadReg<const volatile uint32_t>;
inline DirectPinRead direct_pin_read(pin_t pin) {
    return {portModeRegister(digitalPinToPort(pin)) + 8,
            static_cast<uint32_t>(digitalPinToBitMask(pin))};
}

// More special cases
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)

using DirectPinRead = DirectPinReadReg<const volatile uint32_t>;
inline DirectPinRead direct_pin_read(pin_t pin) {
    return {portOutputRegister(pin), digitalPinToBitMask(pin)};
}

// Nordic
#elif defined(RBL_NRF51822) || defined(ARDUINO_ARCH_NRF52840)

struct DirectPinRead {
    uint32_t pin;
    bool read() const { return nrf_gpio_pin_read(pin); }
};
inline DirectPinRead direct_pin_read(pin_t pin) {
#if defined(RBL_NRF51822)
    return {static_cast<uint32_t>(pin)};
#elif defined(ARDUINO_ARCH_NRF52840)
    return {static_cast<uint32_t>(digitalPinToPinName(pin))};
#else
#error "Unknown Nordic board"
#endif
}

// Raspberry Pi RP2040
#elif defined(ARDUINO_ARCH_RP2040)

END_CS_NAMESPACE
#include <hardware/structs/sio.h>
BEGIN_CS_NAMESPACE
using DirectPinRead = DirectPinReadReg<io_ro_32>;
inline DirectPinRead direct_pin_read(pin_t pin) {
    return {&sio_hw->gpio_in, uint32_t(1) << pin};
}

// ARM mbed OS
#elif defined(ARDUINO_ARCH_MBED)

END_CS_NAMESPACE
#include <mbed.h>
// ↑ Must be first
#include <drivers/DigitalIn.h>
BEGIN_CS_NAMESPACE
struct DirectPinRead {
    mbed::DigitalIn pin;
    bool read() { return pin.read(); }
};
inline DirectPinRead direct_pin_read(pin_t pin) {
    return {mbed::DigitalIn {digitalPinToPinName(pin), PullUp}};
}

// Arduino UNO R4
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)

struct DirectPinRead {
    bsp_io_port_pin_t pin;
    bool read() const {
        bsp_io_level_t ret;
        R_IOPORT_PinRead(NULL, pin, &ret);
        return ret == BSP_IO_LEVEL_LOW;
    }
};
inline DirectPinRead direct_pin_read(pin_t pin) { return {g_pin_cfg[pin].pin}; }

#else
#warning "Unknown board. Please define the direct_pin_read function."

struct DirectPinRead {
    uint8_t pin;
    bool read() const { return ::digitalRead(pin) == HIGH; }
};
inline DirectPinRead direct_pin_read(uint8_t pin) { return {pin}; }

#endif

END_CS_NAMESPACE