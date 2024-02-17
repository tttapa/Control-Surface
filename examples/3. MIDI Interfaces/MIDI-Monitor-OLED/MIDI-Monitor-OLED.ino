/** 
 * This example shows all incoming and outgoing MIDI messages on an SSD1306
 * OLED display. This can be handy for debugging purposes when you don't have
 * access to the Serial Monitor.
 *
 * @boards  AVR, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Teensy 3.x, ESP32, ESP8266
 *
 * Connections
 * -----------
 * 
 * - 5:  Push button (to ground)
 * - MOSI: OLED Data/D1 (SPI)
 * - SCK:  OLED Clock/D0 (SPI)
 * - 8:  OLED Reset
 * - 9:  OLED Data/Command
 * - 10: OLED %Cable Select
 *
 * Behavior
 * --------
 * 
 * - When sending MIDI messages to the Arduino using the USB interface, these
 *   messages are printed on the OLED display, prefixed by a right arrow (→).
 * - If any MIDI messages are sent by Control Surface (e.g. by pressing the push
 *   button), these messages are printed on the OLED display, prefixed
 *   by a left arrow (←).  
 *   @image html midi-monitor-oled.gif
 * 
 * Explanation
 * -----------
 * 
 * The code below defines the `OLEDDebugMIDI_Output` class, which implements the
 * @ref MIDI_Sink and @ref MIDI_Sender interfaces. This means that it can accept
 * MIDI messages coming from @ref MIDI_Pipe%s, and it provides all of the usual
 * functions for sending MIDI messages.
 *
 * The MIDI over USB interface (`midi`) will be used as the primary MIDI
 * interface (as in the other Control Surface examples).  
 * Two instances of our `OLEDDebugMIDI_Output` class are created: one for
 * printing messages that are received by the primary MIDI interface, and one
 * for printing messages that are sent by Control Surface.  
 * The MIDI interfaces and Control Surface itself are connected appropriately
 * using some pipes.
 *
 * <img style="margin: 0 auto; display: block; width: 38em" src="midi-pipes-mon-oled.svg"/>
 *
 * @see @ref MIDI_Pipes-Routing.ino
 * @see @ref Debug-MIDI-Interface.ino
 * @see @ref midi_md-routing "MIDI Tutorial: Routing MIDI messages"
 * @see @ref MIDI-Monitor.ino
 * 
 * Written by PieterP, 2024-01-05  
 * https://github.com/tttapa/Control-Surface
 */

#include <Adafruit_SSD1306.h>
#include <Control_Surface.h>

// Specify the options and pins for the display
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr int8_t OLED_reset = 8; // Reset pin of the display
constexpr int8_t OLED_DC = 9;    // Data/Command pin of the display
constexpr int8_t OLED_CS = 10;   // Chip Select pin of the display
constexpr uint32_t SPI_Frequency = SPI_MAX_SPEED;

Adafruit_SSD1306 disp {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS,       SPI_Frequency,
};

void init_display() {
  if (!disp.begin())
    FATAL_ERROR(F("SSD1306 allocation failed."), 0x1306);
  disp.setTextColor(SSD1306_WHITE);
  disp.setTextSize(1);
  disp.cp437();
  disp.clearDisplay();
  disp.display();
  disp << uppercase;
}

// Shifts the entire display up by the given number of lines and sets the
// cursor at the beginning of the first of the newly created empty lines.
void shift_display(uint8_t lines = 1) {
  auto buf = disp.getBuffer();
  auto end = buf + SCREEN_WIDTH * SCREEN_HEIGHT / 8;
  std::move(buf + lines * SCREEN_WIDTH, end, buf);
  std::fill(end - lines * SCREEN_WIDTH, end, 0);
  disp.setCursor(0, SCREEN_HEIGHT - 8 * lines);
}

constexpr const char *abbrev_midi_status_names[] {
  "Note Off", "Note On", "Key Pres",  "Ctrl Ch",
  "Prog Ch",  "Ch Pres", "Pitch Bend"};

class OLEDDebugMIDI_Output : public MIDI_Sender<OLEDDebugMIDI_Output>,
                             public TrueMIDI_Sink {
 public:
  OLEDDebugMIDI_Output(const char *prefix = nullptr) : prefix(prefix) {}

  // Implement the required MIDI_Sink interface
  void sinkMIDIfromPipe(ChannelMessage m) override { send(m); }
  void sinkMIDIfromPipe(SysExMessage m) override { send(m); }
  void sinkMIDIfromPipe(SysCommonMessage m) override { send(m); }
  void sinkMIDIfromPipe(RealTimeMessage m) override { send(m); }

 private:
  // Implement the required MIDI_Sender interface
  friend MIDI_Sender<OLEDDebugMIDI_Output>;
  void sendChannelMessageImpl(ChannelMessage msg) {
    uint8_t messageType = (msg.header >> 4) - 8;
    if (messageType >= 7)
      return;
    shift_display(1);
    if (prefix != nullptr)
      disp << prefix << ' ';
    if (print_cable)
      disp << msg.getCable().getOneBased() << ':';
    disp << msg.getChannel().getOneBased() << ' ';
    disp << abbrev_midi_status_names[messageType];
    if (msg.getMessageType() == msg.PitchBend)
      disp << ' ' << hex << msg.getData14bit() << 'h' << dec;
    else if (msg.hasTwoDataBytes())
      disp << ' ' << hex << msg.getData1() << 'h' << ' ' << msg.getData2()
           << 'h' << dec;
    else
      disp << abbrev_midi_status_names[messageType] << ' ' << hex
           << msg.getData1() << 'h' << dec;
    disp.display();
  }
  void sendSysCommonImpl(SysCommonMessage msg) {
    shift_display(1);
    if (prefix != nullptr)
      disp << prefix << ' ';
    if (print_cable)
      disp << msg.getCable().getOneBased() << ' ';
    disp << msg.getMessageType() << hex;
    if (msg.getNumberOfDataBytes() >= 1)
      disp << ' ' << msg.getData1() << 'h';
    if (msg.getNumberOfDataBytes() >= 2)
      disp << ' ' << msg.getData2() << 'h';
    disp << dec;
    disp.display();
  }
  void sendSysExImpl(SysExMessage msg) {
    const uint16_t bytes_per_line = 7;
    uint8_t lines = (msg.length + bytes_per_line - 1) / bytes_per_line;
    shift_display(1 + lines);
    if (prefix != nullptr)
      disp << prefix << ' ';
    if (print_cable)
      disp << msg.getCable().getOneBased() << ' ';
    disp << F("Sys Ex [") << msg.length << (msg.isLastChunk() ? "]" : "+]");
    uint16_t i = 0;
    while (i < msg.length) {
      uint16_t rem = msg.length - i;
      uint16_t len = std::min(bytes_per_line, rem);
      disp.println();
      disp << ' ' << AH::HexDump(msg.data + i, len);
      i += bytes_per_line;
    }
    disp.display();
  }
  void sendRealTimeImpl(RealTimeMessage msg) {
    shift_display(1);
    if (prefix != nullptr)
      disp << prefix << ' ';
    if (print_cable)
      disp << msg.getCable().getOneBased() << ' ';
    disp << msg.getMessageType();
    disp.display();
  }
  void sendNowImpl() {}

  // Prefix is printed in front of every message
  const char *prefix;
  // Print the USB cable number
  bool print_cable = false;
};

// Main MIDI interface used by Control Surface
USBMIDI_Interface midi;
// Prints messages being received to the display
OLEDDebugMIDI_Output midi_disp_in {"\x1a"}; // prefix="→"
// Prints messages being sent to the display
OLEDDebugMIDI_Output midi_disp_out {"\x1b"}; // prefix="←"
// Two pipes for MIDI routing
MIDI_PipeFactory<2> pipes;

// MIDI element to send messages for testing
NoteButton button {5, MIDI_Notes::C[4]};

void setup() {
  init_display();
  midi.setAsDefault();
  Control_Surface.begin();
  // Route MIDI messages to the display
  midi >> pipes >> midi_disp_in;
  Control_Surface >> pipes >> midi_disp_out;
}

void loop() {
  Control_Surface.loop();
}
