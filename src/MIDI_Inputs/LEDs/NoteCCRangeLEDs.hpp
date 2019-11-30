#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR()

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <MIDI_Inputs/NoteCCRange.hpp>

BEGIN_CS_NAMESPACE

template <uint8_t NumLEDs>
class NoteCCLED : public SimpleNoteCCValueCallback {
  public:
    NoteCCLED(const PinList<NumLEDs> &ledPins) : ledPins(ledPins) {}

    void setThreshold(uint8_t threshold) { this->threshold = threshold; }
    uint8_t getThreshold() const { return this->threshold; }

    void begin(const INoteCCValue &t) override {
        for (pin_t pin : ledPins)
            AH::ExtIO::pinMode(pin, OUTPUT);
        updateAll(t);
    }

    void update(const INoteCCValue &t, uint8_t index) override {
        uint8_t value = t.getValue(index);
        bool state = value > threshold;
        AH::ExtIO::digitalWrite(ledPins[index], state);
    }

  private:
    PinList<NumLEDs> ledPins;
    uint8_t threshold = 0x3F;
};

template <uint8_t RangeLen>
class NoteRangeLEDs : public GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                                                NoteCCLED<RangeLen>> {
  public:
    NoteRangeLEDs(const PinList<RangeLen> &ledPins,
                  MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCLED<RangeLen>>{
              address,
              {ledPins},
          } {}
};

class NoteValueLED
    : public GenericNoteCCRange<MIDIInputElementNote, 1, NoteCCLED<1>> {
  public:
    NoteValueLED(pin_t ledPin, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, 1, NoteCCLED<1>>{
              address,
              {{ledPin}},
          } {}
};

using MIDINoteLED[[deprecated("Use NoteValueLED instead")]] = NoteValueLED;

template <uint8_t RangeLen>
class CCRangeLEDs : public GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                                              NoteCCLED<RangeLen>> {
  public:
    CCRangeLEDs(const PinList<RangeLen> &ledPins, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, NoteCCLED<RangeLen>>{
              address,
              {ledPins},
          } {}
};

class CCValueLED
    : public GenericNoteCCRange<MIDIInputElementCC, 1, NoteCCLED<1>> {
  public:
    CCValueLED(pin_t ledPin, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, 1, NoteCCLED<1>>{
              address,
              {{ledPin}},
          } {}
};

namespace Bankable {

template <uint8_t RangeLen, uint8_t BankSize>
class NoteRangeLEDs : public GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                                                BankSize, NoteCCLED<RangeLen>> {
  public:
    NoteRangeLEDs(const BankConfig<BankSize> &config,
                  const PinList<RangeLen> &ledPins,
                  const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                             NoteCCLED<RangeLen>>{
              config,
              address,
              {ledPins},
          } {}
};

template <uint8_t BankSize>
class NoteValueLED : public GenericNoteCCRange<MIDIInputElementNote, 1,
                                               BankSize, NoteCCLED<1>> {
  public:
    NoteValueLED(const BankConfig<BankSize> &config, pin_t ledPin,
                 const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementNote, 1, BankSize, NoteCCLED<1>>{
              config,
              address,
              {{ledPin}},
          } {}
};

template <uint8_t BankSize>
using MIDINoteLED[[deprecated("Use NoteValueLED instead")]] =
    NoteValueLED<BankSize>;

template <uint8_t RangeLen, uint8_t BankSize>
class CCRangeLEDs : public GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                                              BankSize, NoteCCLED<RangeLen>> {
  public:
    CCRangeLEDs(const BankConfig<BankSize> &config,
                const PinList<RangeLen> &ledPins,
                const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                             NoteCCLED<RangeLen>>{
              config,
              address,
              {ledPins},
          } {}
};

template <uint8_t BankSize>
class CCValueLED
    : public GenericNoteCCRange<MIDIInputElementCC, 1, BankSize, NoteCCLED<1>> {
  public:
    CCValueLED(const BankConfig<BankSize> &config, pin_t ledPin,
               const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementCC, 1, BankSize, NoteCCLED<1>>{
              config,
              address,
              {{ledPin}},
          } {}
};

} // namespace Bankable

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
