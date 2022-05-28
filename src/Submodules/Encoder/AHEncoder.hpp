#pragma once

#include "AtomicPosition.hpp"
#include "DirectPinRead.hpp"
#include "NumInterrupts.hpp"

BEGIN_CS_NAMESPACE

// Use IRAM_ATTR for ISRs to prevent ESP8266 resets
#if defined(ESP8266) || defined(ESP32)
#define CS_ENCODER_ISR_ATTR IRAM_ATTR
#else
#define CS_ENCODER_ISR_ATTR
#endif

// Largest interrupt number.
#define CS_ENCODER_ARGLIST_SIZE CORE_NUM_INTERRUPT

/// Class for reading quadrature encoders, heavily influenced by
/// http://www.pjrc.com/teensy/td_libs_Encoder.html
class AHEncoder {
  public:
    /**
     * @brief   Constructor.
     * 
     * @param   pinA
     *          The first pin, connected to the encoder's "A" pin.
     * @param   pinB
     *          The second pin, connected to the encoder's "B" pin.
     * 
     * The internal pull-up resistors will be enabled on these pins upon
     * initialization (when calling the `begin` method).
     */
    AHEncoder(uint8_t pinA, uint8_t pinB);

    /// Copy constructor: copying an Encoder object is semantically meaningless,
    /// so it has been deleted.
    AHEncoder(const AHEncoder &) = delete;
    /// Copy assignment: copying an Encoder object is semantically meaningless,
    /// so it has been deleted.
    AHEncoder &operator=(const AHEncoder &) = delete;
    /// Swap two Encoder objects.
    friend void swap(AHEncoder &a, AHEncoder &b);

    /// Move constructor.
    AHEncoder(AHEncoder &&other);
    /// Move assignment.
    AHEncoder &operator=(AHEncoder &&other);

    /// Destructor, detaches the interrupts.
    ~AHEncoder();

    /// Initialize this encoder by enabling the pull-up resistors and attaching
    /// the interrupts handlers (if interrupts are enabled and available on the
    /// the given pins).
    void begin();
    /// Disable the interrupts used by this encoder.
    void end();

  public:
    /// Read the current absolute position of the encoder.
    int32_t read();
    /// Read the current absolute position of the encoder and reset it to zero
    /// afterwards.
    int32_t readAndReset(int32_t newpos = 0);
    /// Set the absolute position to the given value.
    void write(int32_t p);

  private:
    void attachInterruptCtx(int interrupt);
    void detachInterruptCtx(int interrupt);

  private:
    uint8_t pins[2];
    uint8_t interrupts_in_use = 0;
    uint8_t state = 0;
    DirectPinRead direct_pins[2];
    AtomicPosition<int32_t> position {0};

  public:
    /// `update()` is not meant to be called from outside Encoder,
    /// but it is public to allow static interrupt routines.
    /// DO NOT call update() directly from sketches.
    CS_ENCODER_ISR_ATTR void update();
    /// Similarly to `update()`, don't use these interrupt handler arguments
    /// from your sketch.
    static AHEncoder *interruptArgs[CS_ENCODER_ARGLIST_SIZE];
};

END_CS_NAMESPACE

#include "AHEncoder.ipp"
