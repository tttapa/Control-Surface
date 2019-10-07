#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <MIDI_Inputs/MIDINoteCCRange.hpp>

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
class NoteRangeLEDs : public MIDINoteCCRange<MIDIInputElementNote, //
                                             RangeLen,             //
                                             NoteCCLED<RangeLen>> {
  public:
    NoteRangeLEDs(const PinList<RangeLen> &ledPins,
                  MIDICNChannelAddress address)
        : MIDINoteCCRange<MIDIInputElementNote, //
                          RangeLen,             //
                          NoteCCLED<RangeLen>>{
              address,
              {ledPins},
          } {}
};

class NoteValueLED : public MIDINoteCCRange<MIDIInputElementNote, //
                                            1,                    //
                                            NoteCCLED<1>> {
  public:
    NoteValueLED(pin_t ledPin, MIDICNChannelAddress address)
        : MIDINoteCCRange<MIDIInputElementNote, //
                          1,                    //
                          NoteCCLED<1>>{
              address,
              {{ledPin}},
          } {}
};

using MIDINoteLED __attribute__((deprecated)) = NoteValueLED;

template <uint8_t RangeLen>
class CCRangeLEDs : public MIDINoteCCRange<MIDIInputElementCC, //
                                           RangeLen,           //
                                           NoteCCLED<RangeLen>> {
  public:
    CCRangeLEDs(const PinList<RangeLen> &ledPins, MIDICNChannelAddress address)
        : MIDINoteCCRange<MIDIInputElementCC, //
                          RangeLen,           //
                          NoteCCLED<RangeLen>>{
              address,
              {ledPins},
          } {}
};

class CCValueLED : public MIDINoteCCRange<MIDIInputElementCC, //
                                          1,                  //
                                          NoteCCLED<1>> {
  public:
    CCValueLED(pin_t ledPin, MIDICNChannelAddress address)
        : MIDINoteCCRange<MIDIInputElementCC, //
                          1,                  //
                          NoteCCLED<1>>{
              address,
              {{ledPin}},
          } {}
};

namespace Bankable {

template <uint8_t RangeLen, uint8_t BankSize>
class NoteRangeLEDs : public MIDINoteCCRange<MIDIInputElementNote, //
                                             RangeLen,             //
                                             BankSize,             //
                                             NoteCCLED<RangeLen>> {
  public:
    NoteRangeLEDs(const BankConfig<BankSize> &config,
                  const PinList<RangeLen> &ledPins,
                  const MIDICNChannelAddress &address)
        : MIDINoteCCRange<MIDIInputElementNote, //
                          RangeLen,             //
                          BankSize,             //
                          NoteCCLED<RangeLen>>{
              config,
              address,
              {ledPins},
          } {}
};

template <uint8_t BankSize>
class NoteValueLED : public MIDINoteCCRange<MIDIInputElementNote, //
                                            1,                    //
                                            BankSize,             //
                                            NoteCCLED<1>> {
  public:
    NoteValueLED(const BankConfig<BankSize> &config, pin_t ledPin,
                 const MIDICNChannelAddress &address)
        : MIDINoteCCRange<MIDIInputElementNote, //
                          1,                    //
                          BankSize,             //
                          NoteCCLED<1>>{
              config,
              address,
              {{ledPin}},
          } {}
};

template <uint8_t BankSize>
using MIDINoteLED __attribute__((deprecated)) = NoteValueLED<BankSize>;

template <uint8_t RangeLen, uint8_t BankSize>
class CCRangeLEDs : public MIDINoteCCRange<MIDIInputElementCC, //
                                           RangeLen,           //
                                           BankSize,           //
                                           NoteCCLED<RangeLen>> {
  public:
    CCRangeLEDs(const BankConfig<BankSize> &config,
                const PinList<RangeLen> &ledPins,
                const MIDICNChannelAddress &address)
        : MIDINoteCCRange<MIDIInputElementCC, //
                          RangeLen,           //
                          BankSize,           //
                          NoteCCLED<RangeLen>>{
              config,
              address,
              {ledPins},
          } {}
};

template <uint8_t BankSize>
class CCValueLED : public MIDINoteCCRange<MIDIInputElementCC, //
                                          1,                  //
                                          BankSize,           //
                                          NoteCCLED<1>> {
  public:
    CCValueLED(const BankConfig<BankSize> &config, pin_t ledPin,
               const MIDICNChannelAddress &address)
        : MIDINoteCCRange<MIDIInputElementCC, //
                          1,                  //
                          BankSize,           //
                          NoteCCLED<1>>{
              config,
              address,
              {{ledPin}},
          } {}
};

} // namespace Bankable

END_CS_NAMESPACE