#ifndef direct_pin_read_h_
#define direct_pin_read_h_

#if defined(__AVR__)

#define IO_REG_TYPE			uint8_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(TEENSYDUINO) && (defined(KINETISK) || defined(KINETISL))

#define IO_REG_TYPE			uint8_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)

#define IO_REG_TYPE			uint32_t
#define PIN_TO_BASEREG(pin)             (portOutputRegister(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(__SAM3X8E__)  // || defined(ESP8266)

#define IO_REG_TYPE			uint32_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(__PIC32MX__)

#define IO_REG_TYPE			uint32_t
#define PIN_TO_BASEREG(pin)             (portModeRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)	(((*(base+4)) & (mask)) ? 1 : 0)

/* ESP8266 v2.0.0 Arduino workaround for bug https://github.com/esp8266/Arduino/issues/1110 */
#elif defined(ESP8266)

#define IO_REG_TYPE			uint32_t
#define PIN_TO_BASEREG(pin)             ((volatile uint32_t *)(0x60000000+(0x318)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

/* ESP32  Arduino (https://github.com/espressif/arduino-esp32) */
#elif defined(ESP32)

#define IO_REG_TYPE			uint32_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(__SAMD21G18A__)

#define IO_REG_TYPE                     uint32_t
#define PIN_TO_BASEREG(pin)             portModeRegister(digitalPinToPort(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*((base)+8)) & (mask)) ? 1 : 0)

#elif defined(__SAMD51__)

#define IO_REG_TYPE                     uint32_t
#define PIN_TO_BASEREG(pin)             portInputRegister(digitalPinToPort(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(RBL_NRF51822)

#define IO_REG_TYPE                     uint32_t
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define DIRECT_PIN_READ(base, pin)      nrf_gpio_pin_read(pin)

#elif defined(ARDUINO_ARCH_MBED)

#include <drivers/DigitalIn.h>
#include <memory>
#define IO_REG_TYPE                     std::unique_ptr<mbed::DigitalIn>
#define PIN_TO_BASEREG(pin)             (nullptr)
#define PIN_TO_BITMASK(pin)             std::make_unique<mbed::DigitalIn>(digitalPinToPinName(pin), PullUp)
#define DIRECT_PIN_READ(base, pin)      (pin)->read()

#elif defined(__arc__) /* Arduino101/Genuino101 specifics */

#include "scss_registers.h"
#include "portable.h"
#include "avr/pgmspace.h"
#define GPIO_ID(pin)                    (g_APinDescription[pin].ulGPIOId)
#define GPIO_TYPE(pin)                  (g_APinDescription[pin].ulGPIOType)
#define GPIO_BASE(pin)                  (g_APinDescription[pin].ulGPIOBase)
#define EXT_PORT_OFFSET_SS              0x0A
#define EXT_PORT_OFFSET_SOC             0x50
#define PIN_TO_BASEREG(pin)             ((volatile uint32_t *)g_APinDescription[pin].ulGPIOBase)
#define PIN_TO_BITMASK(pin)             pin
#define IO_REG_TYPE                     uint32_t
static inline __attribute__((always_inline))
IO_REG_TYPE directRead(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    IO_REG_TYPE ret;
    if (SS_GPIO == GPIO_TYPE(pin)) {
        ret = READ_ARC_REG(((IO_REG_TYPE)base + EXT_PORT_OFFSET_SS));
    } else {
        ret = MMIO_REG_VAL_FROM_BASE((IO_REG_TYPE)base, EXT_PORT_OFFSET_SOC);
    }
    return ((ret >> GPIO_ID(pin)) & 0x01);
}
#define DIRECT_PIN_READ(base, pin)      directRead(base, pin)

#endif

#endif
