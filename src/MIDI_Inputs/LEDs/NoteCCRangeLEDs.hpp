#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR()

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Math/IncreaseBitDepth.hpp>
#include <MIDI_Inputs/NoteCCRange.hpp>

BEGIN_CS_NAMESPACE

/// Callback for Note or CC range or value input that displays the value to a
/// normal on/off LED.
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

/// Callback for Note or CC range or value input that displays the value to a
/// PWM LED.
template <uint8_t NumLEDs>
class NoteCCLEDPWM : public SimpleNoteCCValueCallback {
  public:
    NoteCCLEDPWM(const PinList<NumLEDs> &ledPins) : ledPins(ledPins) {}

    void begin(const INoteCCValue &t) override {
        for (pin_t pin : ledPins)
            AH::ExtIO::pinMode(pin, OUTPUT);
        updateAll(t);
    }

    void update(const INoteCCValue &t, uint8_t index) override {
        uint8_t value = AH::increaseBitDepth<8, 7, uint8_t>(t.getValue(index));
        AH::ExtIO::analogWrite(ledPins[index], value);
    }

  private:
    PinList<NumLEDs> ledPins;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //
// Everything below is just definitions of type aliases to make the library
// easier to use.
//
// It defines MIDI elements that listen to (a single, a range of)
// (MIDI Note, MIDI Control Change) message(s) that display the values of
// these messages using normal on/off or PWM LEDs.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

// --------------------------------- ON/OFF --------------------------------- //

/**
 * @brief   MIDI Input Element that listens for MIDI Note messages in a given 
 *          range, and displays their values by turning on/off LEDs depending on
 *          whether the MIDI note velocity is above or below a certain 
 *          threshold.
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI note numbers to listen for.
 */
template <uint8_t RangeLen>
class NoteRangeLEDs : public GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                                                NoteCCLED<RangeLen>> {
  public:
    /**
     * @brief   Construct a new NoteRangeLEDs object.
     * 
     * @param   ledPins 
     *          A list of pin numbers with the LEDs connected.
     * @param   address 
     *          The first address in the range. The entire range is defined by
     *          this address and the `RangeLen` template parameter.  
     *          For example, if `address = 10` and `RangeLen = 4`, the this
     *          object will listen to addresses 10, 11, 12, 13.
     */
    NoteRangeLEDs(const PinList<RangeLen> &ledPins,
                  MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCLED<RangeLen>>{
              address,
              {ledPins},
          } {}
};

/**
 * @brief   MIDI Input Element that listens for MIDI Note messages for the given
 *          note, and displays its value by turning on/off an LED depending on
 *          whether the MIDI note velocity is above or below a certain 
 *          threshold.
 */
class NoteValueLED
    : public GenericNoteCCRange<MIDIInputElementNote, 1, NoteCCLED<1>> {
  public:
    /**
     * @brief   Construct a new NoteValueLED object.
     * 
     * @param   ledPin 
     *          The pin number with the LED connected.
     * @param   address 
     *          The MIDI address to listen to.
     */
    NoteValueLED(pin_t ledPin, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, 1, NoteCCLED<1>>{
              address,
              {{ledPin}},
          } {}
};

/// Deprecated.
/// @see    NoteValueLED
using MIDINoteLED[[deprecated("Use NoteValueLED instead")]] = NoteValueLED;

/**
 * @brief   MIDI Input Element that listens for MIDI Control Change messages in 
 *          a given range, and displays their values by turning on/off LEDs 
 *          depending on whether the controller value is above or below a 
 *          certain threshold.
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI controllers to listen for.
 */
template <uint8_t RangeLen>
class CCRangeLEDs : public GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                                              NoteCCLED<RangeLen>> {
  public:
    /**
     * @brief   Construct a new CCRangeLEDs object.
     * 
     * @param   ledPins 
     *          A list of pin numbers with the LEDs connected.
     * @param   address 
     *          The first address in the range. The entire range is defined by
     *          this address and the `RangeLen` template parameter.  
     *          For example, if `address = 10` and `RangeLen = 4`, the this
     *          object will listen to addresses 10, 11, 12, 13.
     */
    CCRangeLEDs(const PinList<RangeLen> &ledPins, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, NoteCCLED<RangeLen>>{
              address,
              {ledPins},
          } {}
};

/**
 * @brief   MIDI Input Element that listens for MIDI Control Change messages for
 *          the given controller, and displays its value by turning on/off an 
 *          LED depending on whether the MIDI controller value is above or below
 *          a certain threshold.
 */
class CCValueLED
    : public GenericNoteCCRange<MIDIInputElementCC, 1, NoteCCLED<1>> {
  public:
    /** 
     * @brief   Construct a new CCValueLED object.
     * 
     * @param   ledPin 
     *          The pin number with the LED connected.
     * @param   address 
     *          The MIDI address to listen to.
     */
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

/// Deprecated.
/// @see    Bankable::NoteValueLED
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

// ----------------------------------- PWM ---------------------------------- //

/**
 * @brief   MIDI Input Element that listens for MIDI Note messages in a given 
 *          range, and displays their velocity values by changing the brightness
 *          of LEDs using PWM (`analogWrite`).
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI note numbers to listen for.
 */
template <uint8_t RangeLen>
class NoteRangeLEDsPWM : public GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                                                NoteCCLEDPWM<RangeLen>> {
  public:
    /**
     * @brief   Construct a new NoteRangeLEDsPWM object.
     * 
     * @param   ledPins 
     *          A list of pin numbers with the LEDs connected.
     * @param   address 
     *          The first address in the range. The entire range is defined by
     *          this address and the `RangeLen` template parameter.  
     *          For example, if `address = 10` and `RangeLen = 4`, the this
     *          object will listen to addresses 10, 11, 12, 13.
     */
    NoteRangeLEDsPWM(const PinList<RangeLen> &ledPins,
                     MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCLEDPWM<RangeLen>>{
              address,
              {ledPins},
          } {}
};

/**
 * @brief   MIDI Input Element that listens for MIDI Note messages for the given
 *          note, and displays its velocity value by changing the brightness
 *          of an LED using PWM (`analogWrite`).
 */
class NoteValueLEDPWM
    : public GenericNoteCCRange<MIDIInputElementNote, 1, NoteCCLEDPWM<1>> {
  public:
    /**
     * @brief   Construct a new NoteValueLEDPWM object.
     * 
     * @param   ledPin 
     *          The pin number with the LED connected.
     * @param   address 
     *          The MIDI address to listen to.
     */
    NoteValueLEDPWM(pin_t ledPin, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, 1, NoteCCLEDPWM<1>>{
              address,
              {{ledPin}},
          } {}
};

/**
 * @brief   MIDI Input Element that listens for MIDI Control Change messages in
 *          a given range, and displays their values by changing the brightness
 *          of LEDs using PWM (`analogWrite`).
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI controllers to listen for.
 */
template <uint8_t RangeLen>
class CCRangeLEDsPWM : public GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                                              NoteCCLEDPWM<RangeLen>> {
  public:
    /**
     * @brief   Construct a new CCRangeLEDsPWM object.
     * 
     * @param   ledPins 
     *          A list of pin numbers with the LEDs connected.
     * @param   address 
     *          The first address in the range. The entire range is defined by
     *          this address and the `RangeLen` template parameter.  
     *          For example, if `address = 10` and `RangeLen = 4`, the this
     *          object will listen to addresses 10, 11, 12, 13.
     */
    CCRangeLEDsPWM(const PinList<RangeLen> &ledPins, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, NoteCCLEDPWM<RangeLen>>{
              address,
              {ledPins},
          } {}
};

/**
 * @brief   MIDI Input Element that listens for MIDI Control Change messages for
 *          the given controller, and displays its value by changing the 
 *          brightness of an LED using PWM (`analogWrite`).
 */
class CCValueLEDPWM
    : public GenericNoteCCRange<MIDIInputElementCC, 1, NoteCCLEDPWM<1>> {
  public:
    /**
     * @brief   Construct a new CCValueLEDPWM object.
     * 
     * @param   ledPin 
     *          The pin number with the LED connected.
     * @param   address 
     *          The MIDI address to listen to.
     */
    CCValueLEDPWM(pin_t ledPin, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, 1, NoteCCLEDPWM<1>>{
              address,
              {{ledPin}},
          } {}
};

namespace Bankable {

template <uint8_t RangeLen, uint8_t BankSize>
class NoteRangeLEDsPWM : public GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                                                BankSize, NoteCCLEDPWM<RangeLen>> {
  public:
    NoteRangeLEDsPWM(const BankConfig<BankSize> &config,
                  const PinList<RangeLen> &ledPins,
                  const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                             NoteCCLEDPWM<RangeLen>>{
              config,
              address,
              {ledPins},
          } {}
};

template <uint8_t BankSize>
class NoteValueLEDPWM : public GenericNoteCCRange<MIDIInputElementNote, 1,
                                               BankSize, NoteCCLEDPWM<1>> {
  public:
    NoteValueLEDPWM(const BankConfig<BankSize> &config, pin_t ledPin,
                 const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementNote, 1, BankSize, NoteCCLEDPWM<1>>{
              config,
              address,
              {{ledPin}},
          } {}
};

template <uint8_t RangeLen, uint8_t BankSize>
class CCRangeLEDsPWM : public GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                                              BankSize, NoteCCLEDPWM<RangeLen>> {
  public:
    CCRangeLEDsPWM(const BankConfig<BankSize> &config,
                const PinList<RangeLen> &ledPins,
                const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                             NoteCCLEDPWM<RangeLen>>{
              config,
              address,
              {ledPins},
          } {}
};

template <uint8_t BankSize>
class CCValueLEDPWM
    : public GenericNoteCCRange<MIDIInputElementCC, 1, BankSize, NoteCCLEDPWM<1>> {
  public:
    CCValueLEDPWM(const BankConfig<BankSize> &config, pin_t ledPin,
               const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementCC, 1, BankSize, NoteCCLEDPWM<1>>{
              config,
              address,
              {{ledPin}},
          } {}
};

} // namespace Bankable

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
