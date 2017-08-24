/*
  Written by Pieter P 22-08-2017
  https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // Include the library
using namespace ExtIO;

// #define DEBUG
USBDebugMIDIOutput dbg(115200); // Instantiate a USB Debug output
#ifdef DEBUG
USBDebugMIDIOutput dbg(115200); // Instantiate a USB Debug output
#else
USBMIDIOutput midiout; // Instantiate a USB MIDI output
#endif

const uint8_t dataPin = 12;
const uint8_t clockPin = 10;
const uint8_t latchPin = 11;

ShiftRegisterOut SR(dataPin, clockPin, latchPin, LSBFIRST, 24);

const uint8_t ledPin = LED_BUILTIN;

const uint8_t REC_RDY = 0;
const uint8_t SOLO = 8;
const uint8_t MUTE = 0x10;
const uint8_t SELECT = 0x18;

const uint8_t PAN = 0x2A;
const uint8_t PLUGIN = 0x2B;
const uint8_t VPOT_SW = 0x20;

const uint8_t VPOT = 0x10;


Digital channelButtons[] = {
  {2, MUTE, 1, 127},
  {3, SOLO, 1, 127},
  {4, VPOT_SW, 1, 127},
  {5, SELECT, 1, 127},
};
const size_t nb_channelButtons = sizeof(channelButtons) / sizeof(Digital);

Digital otherButtons[] = {
  {6, PAN, 1, 127},
  {7, PLUGIN, 1, 127},
};
const size_t nb_otherButtons = sizeof(otherButtons) / sizeof(Digital);

Bank channelBank;
BankSelector bs(channelBank, { 8, 9 }, {
  SR.red(0),
  SR.red(1),
  SR.red(2),
  SR.red(3),
  SR.red(4),
  SR.red(5),
  SR.red(6),
  SR.red(7),
});                             // INCREMENT_DECREMENT_LEDS

Bank otherBank;

RotaryEncoder encoder(0, 1, VPOT, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


class MIDI_InputHandler {
  public:
    struct input_element {
      input_element* next = nullptr;
      MIDI_Input_Element* element;
    };
    void init() {
      for (input_element *node = firstInputElement; node != nullptr; node = node->next) {
        node->element->init();
      }
    };
    void refresh() {
      if (MIDI_In.available() == 0)
        return;
      do {
        uint8_t *midimsg = MIDI_In.read();
        uint8_t header = midimsg[0];
        uint8_t data1 = midimsg[1];
        uint8_t data2 = midimsg[2];
        Serial.print("New midi message:\t");
        Serial.printf("%02X %02X %02x\r\n", header, data1, data2);
        if (((header & 0b11110000) == NOTE_ON) || ((header & 0b11110000) == NOTE_OFF)) {
          for (input_element *node = firstInputElement; node != nullptr; node = node->next) {
            if (node->element->update(header, data1, data2)) {
              Serial.println("Match");
              break;
            }
          }
        }
      } while (MIDI_In.available() > 0);
    };
    void add(MIDI_Input_Element& element) {
      add(&element);
    };
    void add(MIDI_Input_Element* element) {
      input_element *newElement = new input_element;
      newElement->element = element;
      newElement->next = nullptr;
      if (firstInputElement == nullptr)
      {
        firstInputElement = newElement;
        lastInputElement = firstInputElement;
      }
      else
      {

        lastInputElement->next = newElement;
        lastInputElement = newElement;
      }
    };
  private:
    input_element* firstInputElement = nullptr, *lastInputElement = nullptr;

};


MIDI_Input_LED muteled(14, MUTE, 1, 8, 1);
MIDI_Input_LED sololed(15, SOLO, 1, 8, 1);

MIDI_InputHandler inputhandler;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  delay(500);
  Serial.println(bs.getMode());
  startMIDI(); // Initialise the MIDI output connection
  SR.begin();

  pinMode(ledPin, OUTPUT);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);

  muteled.print();
  sololed.print();

  inputhandler.add(muteled);
  inputhandler.add(sololed);
  inputhandler.init();

  for (uint8_t i = 0; i < nb_channelButtons; i++)
    channelBank.add(channelButtons[i], Bank::CHANGE_ADDRESS);
  channelBank.add(encoder, Bank::CHANGE_ADDRESS);

  bs.setBankSettingChangeEvent(settingChange);
  bs.init();

  for (uint8_t i = 0; i < nb_otherButtons; i++)
    otherBank.add(otherButtons[i]);
}






void loop()
{
  bs.refresh();
  otherBank.refresh();
  MIDI_In.refresh();
  inputhandler.refresh();
}

void settingChange(uint8_t newSetting) {
  Serial.print("New setting:\t");
  Serial.println(newSetting);
  muteled.setAddressOffset(newSetting);
  muteled.refresh();
  sololed.setAddressOffset(newSetting);
  sololed.refresh();
}

void printMidi(uint8_t* midi) {
  printHex((uint8_t) midi[0]);
  Serial.print(' ');
  printHex((uint8_t) midi[1]);
  Serial.print(' ');
  printHex((uint8_t) midi[2]);
  Serial.println();
}
void printHex(uint8_t val) {
  printHexNibble(val >> 4);
  printHexNibble(val);
}
void printHexNibble(uint8_t val) {
  val &= 0b1111;
  Serial.write(val >= 10 ? val + 'A' - 10 : val + '0');
}


void OnNoteOn(byte channel, byte note, byte velocity)
{
  digitalWrite(ledPin, HIGH);
}

void OnNoteOff(byte channel, byte note, byte velocity)
{
  digitalWrite(ledPin, LOW);
}

