/**
 * Code adapted from https://github.com/jdachtera/node-rtpmidi/blob/master/examples/rtpmidi-native-bridge.js
 * 
 * This application will initiate a RTP MIDI connection with the ESP32, create
 * virtual MIDI ports, and bridge the MIDI traffic between the RTP MIDI 
 * connection and the virtual MIDI ports.  
 * You can then connect your DAW or other MIDI application to the virtual MIDI 
 * ports.
 * 
 * To install the dependencies, see the instructions in the Arduino sketch
 * documentation, one level up.
 */

const midi = require('midi');
const rtpmidi = require('rtpmidi');

const input = new midi.input();
const output = new midi.output();

const session = rtpmidi.manager.createSession({
  localName: 'Session 1',
  bonjourName: 'Node RTPMidi',
  port: 5004,
});

// Enable some console output;
// rtpmidi.log.level = 4;

// Create the virtual midi ports
input.openVirtualPort('RTP MIDI Virtual Input');
output.openVirtualPort('RTP MIDI Virtual Output');

// Route the messages
session.on('message', (deltaTime, message) => {
  // message is a Buffer so we convert it to an array to pass it to the midi output.
  const commands = Array.prototype.slice.call(message, 0);
  // console.log('received a network message', commands);
  output.sendMessage(commands);
});

input.on('message', (deltaTime, message) => {
  // console.log('received a local message', message);
  session.sendMessage(deltaTime, message);
});

// Connect to a remote session
session.connect({ address: 'ESP32.local', port: 5004 });
//                            └─── this name should match the AppleMIDI name in
//                                 your Arduino sketch