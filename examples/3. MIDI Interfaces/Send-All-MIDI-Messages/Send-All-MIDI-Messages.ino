/**
 * Example showing how to send any MIDI message.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
 *
 * See the @ref MIDI_Sender documentation for a full overview of all MIDI 
 * sending functions and their arguments.
 *
 * **Expected output**
 * ~~~text
 * Note Off         Channel: 6 Data 1: 0x3c  Data 2: 0x7f
 * Note On          Channel: 6 Data 1: 0x3c  Data 2: 0x7f
 * Key Pressure     Channel: 6 Data 1: 0x3c  Data 2: 0x40
 * Control Change   Channel: 2 Data 1: 0x07  Data 2: 0x78
 * Program Change   Channel: 9 Data 1: 0x06
 * Channel Pressure Channel: 3 Data 1: 0x40
 * Pitch Bend       Channel: 3 Data 1: 0x7f  Data 2: 0x7f (16383)
 * System Common    MTCQuarterFrame    Data 1: 0x2f
 * System Common    SongPositionPointer    Data 1: 0x10  Data 2: 0x4e (10000)
 * System Common    SongSelect Data 1: 0x46
 * System Common    TuneRequest
 * System Exclusive [10] F0 00 01 02 03 04 05 06 07 F7
 * System Exclusive [6+] F0 10 11 12 13 14
 * System Exclusive [6+] 15 16 17 18 19 1A
 * System Exclusive [6]  1B 1C 1D 1E 1F F7
 * Real-Time        TimingClock
 * Real-Time        Start
 * Real-Time        Continue
 * Real-Time        Stop
 * Real-Time        ActiveSensing
 * Real-Time        SystemReset
 * ~~~
 *
 * @see @ref midi_md-sending "MIDI Tutorial: Sending MIDI messages"
 *
 * Written by PieterP, 2021-06-28  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

USBDebugMIDI_Interface midi;

void setup() {
  midi.begin();
  while (!Serial)
    ;

  { // Channel voice messages
    MIDIAddress note = {MIDI_Notes::C[4], Channel_6};
    uint8_t velocity = 127;
    midi.sendNoteOff(note, velocity);
    midi.sendNoteOn(note, velocity);
    uint8_t pressure = 64;
    midi.sendKeyPressure(note, pressure);
    MIDIAddress controller = {MIDI_CC::Channel_Volume, Channel_2};
    uint8_t value = 120;
    midi.sendControlChange(controller, value);
    MIDIAddress program = {MIDI_PC::Harpsichord, Channel_9};
    midi.sendProgramChange(program);
    Channel channel = Channel_3;
    midi.sendChannelPressure(channel, pressure);
    uint16_t bend = 16383;
    midi.sendPitchBend(channel, bend);
  }

  { // System common messages
    uint8_t messageType = 2;
    uint8_t value = 15;
    midi.sendMTCQuarterFrame(messageType, value);
    uint16_t spp = 10000;
    midi.sendSongPositionPointer(spp);
    uint8_t song = 70;
    midi.sendSongSelect(song);
    midi.sendTuneRequest();
  }

  { // System exclusive messages
    uint8_t sysex[] = {0xF0, 0x00, 0x01, 0x02, 0x03,
                       0x04, 0x5,  0x06, 0x07, 0xF7};
    midi.sendSysEx(sysex);
    uint8_t chunk1[] = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14};
    uint8_t chunk2[] = {0x15, 0x16, 0x17, 0x18, 0x19, 0x1A};
    uint8_t chunk3[] = {0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0xF7};
    midi.sendSysEx(chunk1);
    midi.sendSysEx(chunk2);
    midi.sendSysEx(chunk3);
  }

  { // Real-time messages
    midi.sendTimingClock();
    midi.sendStart();
    midi.sendContinue();
    midi.sendStop();
    midi.sendActiveSensing();
    midi.sendSystemReset();
  }
}

void loop() {
  midi.update(); // discard incoming messages
}