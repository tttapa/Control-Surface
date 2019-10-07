#include <Control_Surface.h>

USBMIDI_Interface midi;

ProgramChanger<3> programChanger = {
  {
    MIDI_PC::Acoustic_Grand_Piano,
    MIDI_PC::Rock_Organ,
    MIDI_PC::Electric_Bass_Pick,
  },
  CHANNEL_1,
};

IncrementDecrementSelector<3> programSelector = {
  programChanger,
  {A0, A1},
  Wrap::Clamp,
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
