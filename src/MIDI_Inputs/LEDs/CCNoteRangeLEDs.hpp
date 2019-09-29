#include <MIDI_Inputs/MIDINoteCCRange.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

BEGIN_CS_NAMESPACE

template <uint8_t NumLEDs>
class NoteCCLED {
  public:
    NoteCCLED(const PinList<NumLEDs> &ledPins) : ledPins(ledPins) {}

    void setThreshold(uint8_t threshold) { this->threshold = threshold; }
    uint8_t getThreshold() const { return this->threshold; }

    template <class T>
    void begin(const T &t) {
        for (pin_t pin : ledPins)
            ExtIO::pinMode(pin, OUTPUT);
        update(t);
    }

    template <class T>
    void update(const T &t, uint8_t index) {
        uint8_t value = t.getValue(index);
        bool state = value > threshold;
        ExtIO::digitalWrite(ledPins[index], state);
    }

    template <class T>
    void update(const T &t) {
        for (uint8_t index = 0; index < t.length(); ++index)
            update(t, index);
    }

  private:
    PinList<NumLEDs> ledPins;
    uint8_t threshold = 0x3F;
};

template <uint8_t RangeLen>
using NoteRangeLEDs = MIDINoteCCRange<MIDIInputElementNote, //
                                      RangeLen,             //
                                      NoteCCLED<RangeLen>>; //

using NoteValueLED = MIDINoteCCRange<MIDIInputElementNote, //
                                     1,                    //
                                     NoteCCLED<1>>;        //

template <uint8_t RangeLen>
using CCRangeLEDs = MIDINoteCCRange<MIDIInputElementCC,   //
                                    RangeLen,             //
                                    NoteCCLED<RangeLen>>; //

using CCValueLED = MIDINoteCCRange<MIDIInputElementCC, //
                                   1,                  //
                                   NoteCCLED<1>>;      //

namespace Bankable {

template <uint8_t RangeLen, uint8_t BankSize>
using NoteRangeLEDs = MIDINoteCCRange<MIDIInputElementNote, //
                                      RangeLen,             //
                                      BankSize,             //
                                      NoteCCLED<RangeLen>>; //

template <uint8_t BankSize>
using NoteValueLED = MIDINoteCCRange<MIDIInputElementNote,  //
                                     1,                     //
                                     BankSize,              //
                                     NoteCCLED<1>>; //

template <uint8_t RangeLen, uint8_t BankSize>
using CCRangeLEDs = MIDINoteCCRange<MIDIInputElementCC,   //
                                    RangeLen,             //
                                    BankSize,             //
                                    NoteCCLED<RangeLen>>; //

template <uint8_t BankSize>
using CCValueLED = MIDINoteCCRange<MIDIInputElementCC, //
                                   1,                  //
                                   BankSize,           //
                                   NoteCCLED<1>>;      //

} // namespace Bankable

END_CS_NAMESPACE