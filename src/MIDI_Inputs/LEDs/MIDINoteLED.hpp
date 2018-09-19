#pragma once

#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <MIDI_Inputs/MIDINote.hpp>

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
    MIDINoteLED(pin_t ledPin, uint8_t note, uint8_t channel = 1)
        : MIDINote_Base(note, channel), MIDINote(note, channel),
          MIDINoteLED_Base(ledPin) {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

template <uint8_t N>
class MIDINoteLED : public MIDINote<N>, public MIDINoteLED_Base {
  public:
    MIDINoteLED(const BankConfig<N> &config, pin_t ledPin, uint8_t note,
                uint8_t channel = 1)
        : MIDINote_Base(note, channel), MIDINote<N>(config, note, channel),
          MIDINoteLED_Base(ledPin) {}
};

} // namespace Bankable