/**
 * This example demonstrates how to use the AppleMIDI library to use Control
 * Surface over the network.
 * 
 * This example requires the 
 * [AppleMIDI](https://github.com/lathoub/Arduino-AppleMIDI-Library) and 
 * [MIDI](https://github.com/FortySevenEffects/arduino_midi_library) libraries.
 * 
 * @boards  ESP32
 * 
 * Connections
 * -----------
 * 
 * The on-board LED will be used, as well as the push button on GPIO0. These 
 * should be present on most ESP32 boards, if this is not the case for your 
 * board, connect an LED (+ series resistor) and a push button to the relevant
 * pins (in the "MIDI Elements" section below). For more details, see the 
 * @ref NoteButton.ino and @ref 1.Note-LED.ino examples.
 * 
 * WiFi Credentials
 * ----------------
 * 
 * Open the tab `WiFi-Credentials.example.h`, enter your WiFi credentials, and
 * rename the file to `WiFi-Credentials.h`.
 * 
 * Behavior
 * --------
 * 
 * Upload the code to the ESP32, and open the Serial monitor. You should see
 * output like this
 * 
 * ~~~
 * Connecting to Your WiFi Network ...
 * Connected!
 * IP address: 192.168.1.35
 * mDNS responder started (ESP32.local)
 * ~~~
 * 
 * Next, connect to the device using your DAW or other MIDI software. If the 
 * software supports mDNS (Apple Bonjour), you can use `ESP32.local`, 
 * otherwise, you'll have to use the IP address.
 * 
 * When the connection is successful, you'll see the following message in the
 * Serial monitor:
 * 
 * ~~~
 * Connected to session Your Session Name
 * ~~~
 * 
 * When the button is pushed, a MIDI note on message for note C4 (middle C) is
 * sent.  
 * When the ESP32 receives a MIDI note message for that note, it turn on/off the
 * LED accordingly.
 * 
 * RTP MIDI Bridge (Linux)
 * -----------------------
 * 
 * If you're on Linux, you can use the rtpmidi-bridge application in the example
 * folder. You'll need to install Node.js and NPM.
 * 
 * First, install the necessary dependencies and build tools:
 * 
 * ~~~sh
 * sudo apt install build-essential libasound2-dev libavahi-compat-libdnssd-dev
 * ~~~
 * 
 * Then install the dependencies using NPM:
 * 
 * ~~~sh
 * npm install
 * ~~~
 * You might get a compilation error for the `avahi_pub` module. This is not an
 * issue, it's an optional dependency of the `rtpmidi` module.
 * 
 * Finally, run the application:
 * 
 * ~~~sh
 * node rtpmidi-bridge.js
 * ~~~
 * 
 * The application will initiate a RTP MIDI connection with the ESP32, create
 * virtual MIDI ports, and bridge the MIDI traffic between the RTP MIDI 
 * connection and the virtual MIDI ports.  
 * You can then connect your DAW or other MIDI application to the virtual MIDI 
 * ports.
 * 
 * When the ESP32 is connected, you should see the following in the serial 
 * monitor and the rtpmidi-bridge output respectively:
 * 
 * ~~~
 * Connected to session Node RTPMidi
 * ~~~
 * 
 * ~~~
 * 2020-05-06T15:50:42.956Z info:  Invitation accepted by ESP32
 * 2020-05-06T15:50:42.962Z info:  Data channel to ESP32 established
 * ~~~
 * 
 * Mapping
 * -------
 * 
 * Connect the virtual MIDI ports or the AppleMIDI connection to a device or 
 * application that can send and receive MIDI notes.
 * 
 * Written by PieterP, 2020-05-06  
 * https://github.com/tttapa/Control-Surface
 */

#define USE_EXT_CALLBACKS
#include <AppleMIDI.h>
USING_NAMESPACE_APPLEMIDI

#include <Control_Surface.h>
#include <MIDI_Interfaces/Wrappers/FortySevenEffects.hpp>

#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "WiFi-Credentials.h" // See instructions above

// ----------------------------- MIDI Interface ----------------------------- //

// First create the AppleMIDI instance
APPLEMIDI_CREATE_INSTANCE(WiFiUDP, MIDI, "ESP32", 5004);
//                           │       │      │       └──── Local port number
//                           │       │      └──────────── Name
//                           │       └─────────────────── MIDI instance name
//                           └─────────────────────────── Network socket class

// Then wrap it in a Control Surface-compatible MIDI interface
FortySevenEffectsMIDI_Interface<decltype(MIDI) &> AppleMIDI_interface = MIDI;

// ------------------------------ MIDI Elements ----------------------------- //

// Add some MIDI elements for testing
NoteButton button {
  0, // GPIO0 has a push button connected on most boards
  MIDI_Notes::C(4), 
};

NoteLED led {
  LED_BUILTIN, // If your board has one, otherwise, specify a pin number here
  MIDI_Notes::C(4),
};

// --------------------------- AppleMIDI callbacks -------------------------- //

void onAppleMidiConnected(const ssrc_t &ssrc, const char *name) {
  Serial << F("Connected to session ") << name << endl;
}
void onAppleMidiDisconnected(const ssrc_t &ssrc) {
  Serial << F("Disconnected") << endl;
}
void onAppleMidiException(const ssrc_t &ssrc, const Exception &e,
                          int32_t value);

// ---------------------------------- Setup --------------------------------- //

void setup() {
  Serial.begin(115200);

  // Connect to the WiFi network
  Serial << endl << F("Connecting to ") << ssid << ' ';
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    Serial.print("."), delay(250);
  Serial << endl
         << F("Connected!") << endl
         << F("IP address: ") << WiFi.localIP() << endl;

  // Set up mDNS responder:
  if (!MDNS.begin(AppleMIDI.getName()))
    FATAL_ERROR(F("Error setting up MDNS responder!"), 0x0032);
  Serial << F("mDNS responder started (") << AppleMIDI.getName() << ".local)"
         << endl;
  MDNS.addService("apple-midi", "udp", AppleMIDI.getPort());

  // Set up some AppleMIDI callback handles
  AppleMIDI.setHandleConnected(onAppleMidiConnected);
  AppleMIDI.setHandleDisconnected(onAppleMidiDisconnected);
  AppleMIDI.setHandleException(onAppleMidiException);

  // Initialize Control Surface (also calls MIDI.begin())
  Control_Surface.begin();
}

// ---------------------------------- Loop ---------------------------------- //

void loop() {
  // Update all MIDI elements and handle incoming MIDI
  Control_Surface.loop();
}

// ----------------------------- Error handling ----------------------------- //

void onAppleMidiException(const ssrc_t &ssrc, const Exception &e,
                          int32_t value) {
  switch (e) {
    case Exception::BufferFullException:
      Serial << F("*** BufferFullException") << endl;
      break;
    case Exception::ParseException:
      Serial << F("*** ParseException") << endl;
      break;
    case Exception::TooManyParticipantsException:
      Serial << F("*** TooManyParticipantsException") << endl;
      break;
    case Exception::UnexpectedInviteException:
      Serial << F("*** UnexpectedInviteException") << endl;
      break;
    case Exception::ParticipantNotFoundException:
      Serial << F("*** ParticipantNotFoundException: ") << value << endl;
      break;
    case Exception::ComputerNotInDirectory:
      Serial << F("*** ComputerNotInDirectory: ") << value << endl;
      break;
    case Exception::NotAcceptingAnyone:
      Serial << F("*** NotAcceptingAnyone: ") << value << endl;
      break;
    case Exception::ListenerTimeOutException:
      Serial << F("*** ListenerTimeOutException") << endl;
      break;
    case Exception::MaxAttemptsException:
      Serial << F("*** MaxAttemptsException") << endl;
      break;
    case Exception::NoResponseFromConnectionRequestException:
      Serial
        << F("*** No response to the connection request. Check the address and "
             "port, and any firewall or router settings. (time)")
        << endl;
      break;
    case Exception::SendPacketsDropped:
      Serial << F("*** SendPacketsDropped: ") << value << endl;
      break;
    case Exception::ReceivedPacketsDropped:
      Serial << F("*** ReceivedPacketsDropped: ") << value << endl;
      break;
    case Exception::UnexpectedParseException:
      Serial << F("*** UnexpectedParseException") << endl;
      break;
    // case Exception::UdpBeginPacketFailed:
    //   Serial << F("*** UdpBeginPacketFailed: ") << value << endl;
    //   break;
    default:
      Serial << F("*** Unknown exception ") << +e << ": " << value << endl;
      break;
  }
}