#ifndef MIDIOUTPUT_H_
#define MIDIOUTPUT_H_

#include "Arduino.h"

#define MIDI_BAUD 31250
#define HAIRLESS_BAUD 115200

const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;

class MIDIOutput
{
public:
  MIDIOutput()
  {
    DefaultMIDIOutput = this;
  }
  virtual void begin(){};
  void send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2); // Send a 3-byte MIDI packet
  void send(uint8_t m, uint8_t c, uint8_t d1);             // Send a 2-byte MIDI packet

  static MIDIOutput *getDefault()
  {
    return DefaultMIDIOutput;
  }

protected:
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2){};
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1){};
  static MIDIOutput *DefaultMIDIOutput;
};

extern void sendMIDI(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2);
extern void sendMIDI(uint8_t m, uint8_t c, uint8_t d1);
extern void startMIDI();

#endif // MIDIOUTPUT_H_