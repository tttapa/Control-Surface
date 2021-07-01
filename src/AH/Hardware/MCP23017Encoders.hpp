#pragma once

#include <AH/Settings/NamespaceSettings.hpp>
#include <AH/Settings/Warnings.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <AH/Arduino-Wrapper.h>
AH_DIAGNOSTIC_POP()

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Hardware/RegisterEncoders.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   Class for reading 8 rotary encoders using a MCP23017 I²C port 
 *          expander.
 * 
 * Encoders are indexed by a number from 0 to 7. Encoder #0 is connected to pins
 * GPA0 and GPA1, Encoder #1 is connected to pins GPA2 and GPA3, ..., Encoder #7
 * is connected to pins GPB6 and GPB7.
 * 
 * @tparam  WireType
 *          The type of the `Wire` I²C driver to use.
 * @tparam  EncoderPositionType
 *          The type used for saving the encoder positions. `int32_t` is the
 *          default because this matches the Encoder library. You can use small
 *          unsigned types such as `uint8_t` or `uint16_t` if you're just 
 *          interrested in the deltas.
 * @tparam  InterruptSafe
 *          Make the `update` method safe to use inside of an interrupt. 
 *          It makes the necessary variables `volatile` and disables interrupts
 *          while reading the positions from the main program.
 */
template <class WireType, class EncoderPositionType = int32_t,
          bool InterruptSafe = false>
class MCP23017Encoders {
  private:
    constexpr static uint8_t I2C_BASE_ADDRESS = 0x20;

    WireType *wire;
    uint8_t address;
    pin_t interrupt_pin;

    using RegisterEncoderType =
        RegisterEncoders<uint16_t, 8, EncoderPositionType, InterruptSafe>;

    RegisterEncoderType encs;

  protected:
    /// Write any data to the MCP23017.
    template <size_t N>
    void writeI2C(const uint8_t (&values)[N]) {
        this->wire->beginTransmission(address);
        this->wire->write(values, N);
        this->wire->endTransmission();
    }

    /**
     * @brief  Write any data to the MCP23017.
     * 
     * @param   addr
     *          The address of the register to write to.
     * @param   values
     *          The values to write.
     */
    template <class... Args>
    void writeI2C(uint8_t addr, Args... values) {
        const uint8_t v[] = {addr, static_cast<uint8_t>(values)...};
        writeI2C(v);
    }

    /// Read the state of all GPIO pins.
    uint16_t readGPIO() {
        // No need to specify the register address, since this was done in the
        // begin method, and the MCP23017 mode was set to Byte mode with
        // IOCON.BANK = 0 (see §3.2.1 in the datasheet).
        //
        // TODO:
        // For some reason, it sometimes seems to mess up though, and it'll read
        // the wrong register, so we'll select the register again (for now).

        writeI2C(0x12); // GPIOA

        this->wire->requestFrom(address, uint8_t(2));
        uint8_t a = this->wire->read();
        uint16_t b = this->wire->read();
        return a | (b << 8);
    }

  public:
    /**
     * @brief   Constructor.
     * 
     * @param   wire
     *          The Wire/I²C interface to use.
     * @param   addr_offset
     *          A number between 0 and 7 reflecting the state of the I2C address
     *          pins of the MCP23017.
     * @param   interrupt_pin
     *          The pin connected to the MCP23017 interrupt pin. This doesn't 
     *          have to be an interrupt pin of the Arduino.  
     *          The interrupt pin of the MCP23017 is configured as open-drain 
     *          active-low output, and the interrupt pins of GPIO banks A and B
     *          are mirrored/OR'ed together. This means that you only need to 
     *          connect a single interrupt pin for each MCP23017, and you can 
     *          connect the interrupt pins of multiple MCP23017s to a single 
     *          Arduino pin.
     */
    MCP23017Encoders(WireType &wire, uint8_t addr_offset = 0,
                     pin_t interrupt_pin = NO_PIN)
        : wire(&wire), address(I2C_BASE_ADDRESS | addr_offset),
          interrupt_pin(interrupt_pin) {}

    /**
     * @brief   Initialize the MCP23017.
     * 
     * The configuration register is set to the right config, all GPIO pins are
     * set to input mode, the pin change interrupts are enabled, and the 
     * internal pull-up resistors are enabled as well.
     * 
     * The interrupt pin on the Arduino is configured as input with the internal
     * pull-up resistor enabled, because the MCP23017's interrupt output is 
     * configured in open-drain mode.
     * 
     * This function does not initialize the I²C interface. You have to call
     * `Wire.begin()` before calling this function.
     */
    void begin() {
        // Set the IOCON register (configuration register)
        writeI2C(0x0A, // IOCON register for BANK=0
                 0b01100100);
        //         │││││││└─ Unimplemented
        //         ││││││└── INTPOL = Active-low
        //         │││││└─── ODR    = Open-drain output (overrides the INTPOL bit)
        //         ││││└──── HAEN   = Disables the MCP23S17 address pins
        //         │││└───── DISSLW = Slew rate enabled
        //         ││└────── SEQOP  = Sequential operation disabled, address pointer does not increment
        //         │└─────── MIRROR = The INT pins are internally connected
        //         └──────── BANK   = The registers are in the same bank (addresses are sequential)

        // Set all GPIO pins to input mode
        writeI2C(0x00,  // IODIRA
                 0xFF,  // input mode for GPIO A
                 0xFF); // input mode for GPIO B

        // Enable all pin change interrupts
        writeI2C(0x04,  // GPINTENA
                 0xFF,  // interrupt enable for GPIO A
                 0xFF); // interrupt enable for GPIO B

        // Enable all internal pullups
        writeI2C(0x0C,  // GPPUA
                 0xFF,  // pullup enable for GPIO A
                 0xFF); // pullup enable for GPIO B

        // Interrupts are configured in open-drain mode, so enable
        // the internal pullup resistor on the Arduino pin that
        // reads the interrupt pin.
        if (interrupt_pin != NO_PIN)
            ExtIO::pinMode(interrupt_pin, INPUT_PULLUP);

        // Set the address pointer to the GPIOA register.
        // This means that subsequent reads will all toggle between the
        // GPIOA and GPIOB register, so we can speedup reading the GPIO
        // by not having to send an opcode/register address each time.
        writeI2C(0x12); // GPIOA

        /// Initialize the state
        encs.reset(readGPIO());
    }

    /**
     * @brief   If the state of the MCP23017's GPIO changed, read the new state
     *          and update the encoder positions.
     * 
     * Can be called from within an ISR on boards that support I²C inside of 
     * ISRs, on the condition that @p InterruptSafe is set to `true`.
     * 
     * Don't call this function both from the ISR and from your main program,
     * only call it from one of the two.
     */
    void update() {
        // Only update if a pin change interrupt happened
        if (interrupt_pin != NO_PIN &&
            ExtIO::digitalRead(interrupt_pin) == HIGH)
            return;
        // Read both GPIO A and B
        uint16_t newstate = readGPIO();
        encs.update(newstate);
    }

    /**
     * @brief   Read the position of the given encoder.
     * 
     * Does not update the state, just reads it from the buffered position.
     * 
     * Don't call this function from within an ISR.
     * 
     * @param   idx
     *          The index of the encoder to read [0, 7].
     */
    EncoderPositionType read(uint8_t idx) const { return encs.read(idx); }

    /**
     * @brief   Read the position of the given encoder and reset it to zero.
     * 
     * Does not update the state, just reads it from the buffered position.
     * 
     * Don't call this function from within an ISR.
     * 
     * @param   idx
     *          The index of the encoder to read [0, 7].
     */
    EncoderPositionType readAndReset(uint8_t idx) {
        return encs.readAndReset(idx);
    }

    /**
     * @brief   Set the position of the given encoder.
     * 
     * Don't call this function from within an ISR.
     * 
     * @param   idx
     *          The index of the encoder to write [0, 7].
     * @param   pos
     *          The position value to write.
     */
    void write(uint8_t idx, EncoderPositionType pos) { encs.write(idx, pos); }

    /**
     * @brief   Proxy to access a single encoder of the 8 encoders managed by
     *          MCP23017Encoders.
     */
    using MCP23017Encoder = typename RegisterEncoderType::Encoder;

    /**
     * @brief   Get a proxy to one of the encoders managed by this 
     *          MCP23017.
     * 
     * @param   index
     *          The index of the encoder to access.
     */
    MCP23017Encoder operator[](uint8_t index) { return encs[index]; }
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
