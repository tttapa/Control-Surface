#pragma once

#include <AH/Hardware/LEDs/LEDs.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

class VPotRingLEDsCallback {
  public:
    VPotRingLEDsCallback(const AH::LEDs<11> &leds) : leds(leds) {}

    template <class T>
    void begin(const T &) {
        leds.begin();
    }

    template <class T>
    void update(const T &t) {
        leds.displayRange(t.getStartOn(), t.getStartOff());
    }

  private:
    const AH::LEDs<11> leds;
};

// -------------------------------------------------------------------------- //

class VPotRingLEDs : public GenericVPotRing<VPotRingLEDsCallback> {
  public:
    VPotRingLEDs(const PinList<11> &ledPins, uint8_t track,
                 MIDICNChannel channelCN = CHANNEL_1)
        : GenericVPotRing<VPotRingLEDsCallback>{track, channelCN, {ledPins}} {}
};

namespace Bankable {

template <setting_t NumBanks>
class VPotRingLEDs : public GenericVPotRing<NumBanks, VPotRingLEDsCallback> {
  public:
    VPotRingLEDs(BankConfig<NumBanks> config, const PinList<11> &ledPins,
                 uint8_t track, MIDICNChannel channelCN = CHANNEL_1)
        : GenericVPotRing<NumBanks, VPotRingLEDsCallback>{
              config, track, channelCN, {ledPins}} {}
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE