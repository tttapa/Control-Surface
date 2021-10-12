/**
 * <https://github.com/tttapa/Control-Surface/issues/40>
 */

#include <Control_Surface.h>

USBDebugMIDI_Interface midi;

Bank<3> bank;
IncrementDecrementSelector<3> bankselector {bank, {2, 3}, Wrap::Wrap};

constexpr auto no_address = MIDIAddress::invalid();
constexpr auto no_channel = MIDIChannelCable::invalid();

Bankable::ManyAddresses::CCPotentiometer<3> ccpots[] {
  {bank, A0, {91, no_address, 72}},
  {bank, A1, {93, 1, 73}},
  {bank, A2, {92, 2, 74}},
  {bank, A3, {95, 5, 70}},
};

Bankable::ManyAddresses::PBPotentiometer<3> pbpot {
  bank,
  A0,
  {no_channel, CHANNEL_1, no_channel},
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
