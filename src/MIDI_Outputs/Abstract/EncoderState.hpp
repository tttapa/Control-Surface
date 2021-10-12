#pragma once

#include <AH/STL/type_traits>
#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// Class to keep track of relative position changes of rotary encoders.
template <class Enc_t>
class EncoderState {
  private:
    using SignedEnc_t = typename std::make_signed<Enc_t>::type;
    int16_t speedMultiply;
    uint8_t pulsesPerStep;
    int16_t remainder = 0;
    Enc_t deltaOffset = 0;

  public:
    EncoderState(int16_t speedMultiply, uint8_t pulsesPerStep)
        : speedMultiply(speedMultiply), pulsesPerStep(pulsesPerStep) {}

    int16_t update(Enc_t encval) {
        // If Enc_t is an unsigned type, integer overflow is well-defined, which
        // is what we want when Enc_t is small and expected to overflow.
        // However, we need it to be signed because we're interested in the
        // delta.
        Enc_t uDelta = encval - deltaOffset;
        if (uDelta == 0)
            return 0;
        int16_t delta = static_cast<SignedEnc_t>(uDelta);
        // Assumption: delta and speedMultiply are relatively small, so
        // multiplication probably won't overflow.
        int16_t multipliedDelta = delta * speedMultiply + remainder;
        int16_t scaledDelta = multipliedDelta / pulsesPerStep;
        remainder = multipliedDelta % pulsesPerStep;
        deltaOffset += uDelta;
        return scaledDelta;
    }
    void setSpeedMultiply(int16_t speedMultiply) {
        // TODO: Is this correct? Is it necessary? What with negative speedMult?
        remainder = remainder * speedMultiply / this->speedMultiply;
        this->speedMultiply = speedMultiply;
    }
    int16_t getSpeedMultiply() const { return this->speedMultiply; }
};

END_CS_NAMESPACE
