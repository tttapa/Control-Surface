/* Encoder Library, for measuring quadrature encoded signals
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 * Copyright (c) 2011,2013 PJRC.COM, LLC - Paul Stoffregen <paul@pjrc.com>
 *
 * April-September 2020 - Pieter P:
 *      make encoders moveable and add begin method for deterministic
 *      initialization, large cleanup
 * Version 1.2 - fix -2 bug in C-only code
 * Version 1.1 - expand to support boards with up to 60 interrupts
 * Version 1.0 - initial release
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once 

#include <AH/Arduino-Wrapper.h>
#include <Settings/NamespaceSettings.hpp>

#include "utility/direct_pin_read.h"
#include "utility/move.h"
#include "utility/interrupt_pins.h"

BEGIN_CS_NAMESPACE

#define ENCODER_ARGLIST_SIZE CORE_NUM_INTERRUPT

/// All the data needed by interrupts is consolidated into this ugly struct
/// to facilitate assembly language optimizing of the speed critical update.
/// The assembly code uses auto-incrementing addressing modes, so the struct
/// must remain in exactly this order.
struct Encoder_internal_state_t {
    volatile IO_REG_TYPE *pin1_register;
    volatile IO_REG_TYPE *pin2_register;
    IO_REG_TYPE pin1_bitmask;
    IO_REG_TYPE pin2_bitmask;
    uint8_t state;
    int32_t position;
};

/// Class for reading quadrature encoders, based on 
/// http://www.pjrc.com/teensy/td_libs_Encoder.html
class Encoder {
  public:
    /**
     * @brief   Constructor.
     * 
     * @param   pin1
     *          The first pin, connected to the encoder's "A" pin.
     * @param   pin2
     *          The second pin, connected to the encoder's "B" pin.
     * 
     * The internal pull-up resistors will be enabled on these pins upon
     * initialization (when calling the `begin` method).
     */
    Encoder(uint8_t pin1, uint8_t pin2);

    /// Copy constructor: copying an Encoder object is semantically meaningless,
    /// so it has been deleted.
    Encoder(const Encoder &) = delete;
    /// Copy assignment: copying an Encoder object is semantically meaningless,
    /// so it has been deleted.
    Encoder &operator=(const Encoder &) = delete;

    /// Move constructor.
    Encoder(Encoder &&other);
    /// Move assignment.
    Encoder &operator=(Encoder &&other);

    /// Destructor.
    ~Encoder();

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
    int32_t readAndReset();
    /// Set the absolute position to the given value.
    void write(int32_t p);

  private:
    void attachInterruptCtx(int interrupt);
    void detachInterruptCtx(int interrupt);

  private:
    Encoder_internal_state_t encoder;
    uint8_t pin1, pin2;
    uint8_t interrupts_in_use = 0;


  public:
    /// `update()` is not meant to be called from outside Encoder,
    /// but it is public to allow static interrupt routines.
    /// DO NOT call update() directly from sketches.
    static void update(Encoder_internal_state_t *arg);
    /// Similarly to `update()`, don't use these interrupt handler arguments
    /// from your sketch.
    static Encoder_internal_state_t *interruptArgs[ENCODER_ARGLIST_SIZE];
};

END_CS_NAMESPACE

#include "Encoder.ipp"
