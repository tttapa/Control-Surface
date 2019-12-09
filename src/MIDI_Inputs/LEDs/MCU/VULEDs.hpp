#pragma once

#include <AH/Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

template <uint8_t NumLEDs>
class VULEDsCallback {
  public:
    VULEDsCallback(const AH::DotBarDisplayLEDs<NumLEDs> &leds) : leds(leds) {}

    template <class T>
    void begin(T &) {
        leds.begin();
    }

    template <class T>
    void update(T &t) {
        uint8_t value = t.getValue();                      // value in [0, 12]
        value = (value * NumLEDs + FLOOR_CORRECTION) / 12; // value in [0, N]
        leds.display(value);
    }

    /// @copydoc    AH::DotBarDisplayLEDs::getMode
    AH::DotBarMode getMode() const { return this->leds.getMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { this->leds.setMode(mode); }
    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { this->leds.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { this->leds.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::toggleMode
    void toggleMode() { this->leds.toggleMode(); }

  private:
    AH::DotBarDisplayLEDs<NumLEDs> leds;

    /// @see    doc/VU-LED-mapping.ods
    constexpr static uint8_t FLOOR_CORRECTION = 5;
};

template <uint8_t NumLEDs>
class VULEDs : public GenericVU<VULEDsCallback<NumLEDs>> {
  public:
    VULEDs(const PinList<NumLEDs> &ledPins, uint8_t track,
           unsigned int decayTime = VUDecay::Default)
        : GenericVU<VULEDsCallback<NumLEDs>>{
              track,
              CHANNEL_1,
              decayTime,
              {ledPins},
          } {}

    VULEDs(const PinList<NumLEDs> &ledPins, uint8_t track,
           MIDICNChannel channelCN, unsigned int decayTime = VUDecay::Default)
        : GenericVU<VULEDsCallback<NumLEDs>>{
              track,
              channelCN,
              decayTime,
              {ledPins},
          } {}

    /// @copydoc    AH::DotBarDisplayLEDs::getMode
    AH::DotBarMode getMode() const { return this->callback.getMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { this->callback.setMode(mode); }
    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { this->callback.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { this->callback.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::toggleMode
    void toggleMode() { this->callback.toggleMode(); }
};

namespace Bankable {

template <uint8_t NumBanks, uint8_t NumLEDs>
class VULEDs : public GenericVU<NumBanks, VULEDsCallback<NumLEDs>> {
  public:
    VULEDs(BankConfig<NumBanks> config, const PinList<NumLEDs> &ledPins,
           uint8_t track, unsigned int decayTime = VUDecay::Default)
        : GenericVU<NumBanks, VULEDsCallback<NumLEDs>>{
              config, track, CHANNEL_1, decayTime, {ledPins},
          } {}

    VULEDs(BankConfig<NumBanks> config, const PinList<NumLEDs> &ledPins,
           uint8_t track, MIDICNChannel channelCN,
           unsigned int decayTime = VUDecay::Default)
        : GenericVU<NumBanks, VULEDsCallback<NumLEDs>>{
              config, track, channelCN, decayTime, {ledPins},
          } {}

    /// @copydoc    AH::DotBarDisplayLEDs::getMode
    AH::DotBarMode getMode() const { return this->callback.getMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { this->callback.setMode(mode); }
    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { this->callback.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { this->callback.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::toggleMode
    void toggleMode() { this->callback.toggleMode(); }
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE