/**
 * An advanced example with Program Change buttons in different Banks.
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

Bank<3> bank(4);
IncrementDecrementSelector<3> bankSelector {bank, {A0, A1}, Wrap::Clamp};

using namespace MIDI_PC;

Bankable::ManyAddresses::PCButton<3> pcbuttons[] {
  {bank,
   2, // pin
   {
     Acoustic_Grand_Piano,
     Church_Organ,
     Acoustic_Guitar_Nylon,
   }},
  {bank,
   3, // pin
   {
     HonkyTonk_Piano,
     Rock_Organ,
     Distortion_Guitar,
   }},
  {bank,
   4, // pin
   {
     Electric_Piano_1,
     Drawbar_Organ,
     Electric_Bass_Pick,
   }},
  {bank,
   5, // pin
   {
     Harpsichord,
     Accordion,
     Slap_Bass_1,
   }},
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
