/**
 * Filtering and modifying MIDI messages that travel through a MIDI_Pipe.
 * 
 * This example demonstrates how to create custom MIDI pipes that can filter out
 * and modify the messages that travel through it. It uses a debug MIDI 
 * interface for demonstration purposes, and loops back only MIDI note and
 * real-time messages, messages of any other type are dropped. Additionally, all
 * MIDI note messages are transposed down an octave, and their channel is set
 * to 5.
 * 
 * To try it out, open the Serial monitor (at 115200 baud) and type `98 3C 7F`
 * (Note On, channel 9, C4, full velocity). When you press enter, you'll get
 * back the following message (note how the channel and pitch changed):
 *
 * ```
 * Note On    Channel: 5    Data 1: 0x30    Data 2: 0x7F
 * ```
 *
 * If you try to send a Pitch Bend message (for example, `E0 12 34`), you'll get
 * no response, because the code filtered it out.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, ESP32, Teensy 3.x
 */

#include <Control_Surface.h>

USBDebugMIDI_Interface midi;

// A unidirectional pipe that connects a MIDI source to a MIDI sink.
// Messages coming from the source are handled by the `mapForwardMIDI()`
// function, where they can be modified or filtered out, and then sent
// to the MIDI sink by calling the `sourceMIDItoSink()` function.
struct MyMIDIFilter : MIDI_Pipe {
  // Pass on only Note On/Off messages, changing the channel to 5 and
  // transposing all notes an octave down.
  void mapForwardMIDI(ChannelMessage msg) override {
    switch (msg.getMessageType()) {
      case MIDIMessageType::NoteOff:  // fallthrough
      case MIDIMessageType::NoteOn:
        msg.setChannel(Channel_5);
        if (msg.data1 >= 12)
          msg.data1 -= 12;
        sourceMIDItoSink(msg);
        break;
      default: break;
    }
  }
  // Do not pass on System Exclusive messages.
  void mapForwardMIDI(SysExMessage) override {}
  // Do not pass on System Common messages.
  void mapForwardMIDI(SysCommonMessage) override {}
  // Pass on Real-Time messages without changes.
  void mapForwardMIDI(RealTimeMessage msg) override {
    sourceMIDItoSink(msg);
  }
};

MyMIDIFilter filter;

void setup() {
  // Loop back the MIDI input to the MIDI output, through the filter:
  // (MIDI input) -> (MIDI filter) -> (MIDI output)
  // (source)     ->    (pipe)     ->        (sink)
      midi        >>    filter     >>         midi;
  midi.begin();
}

void loop() {
  midi.update();
}
