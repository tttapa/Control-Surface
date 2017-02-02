#ifndef direct_pin_read_h_
#define direct_pin_read_h_

#if defined(__AVR__) || defined(__MK20DX128__) || defined(__MK20DX256__)

#define IO_REG_TYPE			uint8_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(__SAM3X8E__)

#define IO_REG_TYPE			uint32_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#elif defined(__PIC32MX__)

#define IO_REG_TYPE			uint32_t
#define PIN_TO_BASEREG(pin)             (portModeRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)	(((*(base+4)) & (mask)) ? 1 : 0)

#endif

#endif
