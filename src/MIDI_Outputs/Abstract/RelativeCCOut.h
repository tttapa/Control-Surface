#pragma once

#include "../MIDI_Control_Element.h"

enum relativeCCmode { TWOS_COMPLEMENT, BINARY_OFFSET, SIGN_MAGNITUDE };

#define REAPER_RELATIVE_1 TWOS_COMPLEMENT
#define REAPER_RELATIVE_2 BINARY_OFFSET
#define REAPER_RELATIVE_3 SIGN_MAGNITUDE

#define TRACKTION_RELATIVE TWOS_COMPLEMENT

#define MACKIE_CONTROL_RELATIVE SIGN_MAGNITUDE

class RelativeCCOut : public MIDI_Control_Element {
  public:
    RelativeCCOut(uint8_t controllerNumber, uint8_t channel,
                  relativeCCmode mode = TWOS_COMPLEMENT); // Constructor

    /** Convert an 8-bit two's complement integer to a 7-bit two's complement
     *  integer. */
    static uint8_t twosComplement7bit(int8_t value);
    /** Convert an 8-bit two's complement integer to 7-bit sign-magnitude
     *  format. */
    static uint8_t twosComplementTo7bitSignedMagnitude(int8_t value);
    /** Convert an 8-bit two's complement integer to a 7-bit value to send over
     *  MIDI. */
    static uint8_t mapRelativeCC(int8_t value, relativeCCmode mode);

  protected:
    virtual void refresh() = 0;
    void send(long delta);

  private:
    /** Check if the encoder position has changed since last
     *  time, if so, send the relative movement over MIDI. */
    void update() override;
    uint8_t controllerNumber, channel;
    relativeCCmode mode;
};