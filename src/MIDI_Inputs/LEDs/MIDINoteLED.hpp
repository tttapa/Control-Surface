#pragma once

#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <MIDI_Inputs/MIDINote.hpp>

BEGIN_CS_NAMESPACE

/** A base class for MIDI note input LED display. */
class MIDINoteLED_Base : virtual public MIDINote_Base {
  protected:
    MIDINoteLED_Base(pin_t ledPin) : ledPin(ledPin) {}

  public:
    void begin() override { ExtIO::pinMode(ledPin, OUTPUT); }

  private:
    void display() const override { ExtIO::digitalWrite(ledPin, getState()); }

    const pin_t ledPin;
};

// -------------------------------------------------------------------------- //

/** A class for MIDI note input LED display. */
class MIDINoteLED : public MIDINote, public MIDINoteLED_Base {
  public:
    /**
     * @brief Construct a new MIDINoteLED object
     * 
     * @param   ledPin
     *          The digital pin with the LED connected.
     * @param   note
     *          The MIDI note number. [0, 127]
     * @param   channel
     *          The MIDI channel. [1, 16] 
     */
    MIDINoteLED(pin_t ledPin, const MIDICNChannelAddress &address)
        : MIDINote_Base(address), MIDINote(address), MIDINoteLED_Base(ledPin) {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

template <uint8_t N>
class MIDINoteLED : public MIDINote<N>, public MIDINoteLED_Base {
  public:
    MIDINoteLED(const BankConfig<N> &config, pin_t ledPin,
                const MIDICNChannelAddress &address)
        : MIDINote_Base(address), MIDINote<N>(config, address),
          MIDINoteLED_Base(ledPin) {}
};

} // namespace Bankable

END_CS_NAMESPACE