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
const uint8_t MUTE = 16;
const uint8_t SELECT = 24;

const uint8_t PAN = 42;
const uint8_t PLUGIN = 43;
const uint8_t VPOT_SW = 32;

const uint8_t VPOT = 16;


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

#include "usb_dev.h"
#include "usb_midi.h"

const size_t bufferSize = 64;

size_t mod(size_t a, size_t b) {
  return ((a % b) + b) % b;
}

class MIDI_Input {
  public:
    size_t available() {
      return mod((writeIndex - readIndex), bufferSize);
    }
    void refresh() {
      if (rx_packet == nullptr) {
        if (!usb_configuration) return;
        rx_packet = usb_rx(MIDI_RX_ENDPOINT);
        if (rx_packet == nullptr) return;
        if (rx_packet->len == 0) {
          usb_free(rx_packet);
          rx_packet = nullptr;
          return;
        }
      }
      size_t index = rx_packet->index;

      uint32_t n = *(uint32_t *)(rx_packet->buf + index);
      uint8_t* address = rx_packet->buf + index;

      index += 4;
      if (index < rx_packet->len) {
        rx_packet->index = index;
      } else {
        usb_free(rx_packet);
        rx_packet = usb_rx(MIDI_RX_ENDPOINT);
      }

      uint8_t type1 = (n & 0b1111) << 4;
      uint8_t type2 = (n >> 8) & 0b11110000;

      if (type1 != type2)
        return;

      if (!(type1 == NOTE_ON || type1 == NOTE_OFF || type1 == CC || type1 == PITCH_BEND))
        return;

      memcpy(&ringbuffer[writeIndex * 3], address + 1, 3);
      writeIndex = writeIndex < bufferSize - 1 ? writeIndex + 1 : 0;
    }
    uint8_t* read() {
      if (writeIndex == readIndex)
        return nullptr;
      uint8_t *packet = ringbuffer + readIndex * 3;
      readIndex = readIndex < bufferSize - 1 ? readIndex + 1 : 0;
      return packet;
    }
  private:
    usb_packet_t *rx_packet = nullptr;
    uint8_t ringbuffer[bufferSize * 3];
    size_t writeIndex = 0, readIndex = 0;

};

MIDI_Input MIDI_In;

class MIDI_Input_Element {
  public:
    template <size_t M, size_t N>
    MIDI_Input_Element(uint8_t (&addresses)[M], uint8_t (&channels)[N])
      : addresses(addresses), channels(channels), nb_addresses(M), nb_channels(N) {};

    MIDI_Input_Element(uint8_t addresses[], uint8_t channels[], size_t nb_addresses, size_t nb_channels)
      : addresses(addresses), channels(channels), nb_addresses(nb_addresses), nb_channels(nb_channels) {};
    virtual void init() {};
    virtual bool update(uint8_t header, uint8_t data1, uint8_t data2) {};

    void setChannelOffset(uint8_t offset) // Set the channel offset
    {
      channelOffset = offset;
    }
    void setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
    {
      addressOffset = offset;
    }

  protected:
    uint8_t channelOffset = 0;
    uint8_t addressOffset = 0;
    uint8_t *channels, *addresses;
    const size_t nb_channels, nb_addresses;
};

class MIDI_Input_LED : public MIDI_Input_Element {
  public:
    template <size_t M, size_t N>
    MIDI_Input_LED(pin_t pin, uint8_t (&addresses)[M], uint8_t (&channels)[N]) : MIDI_Input_Element(addresses, channels, M, N), pin(pin) {
      states = (uint8_t*)malloc((nb_channels * nb_addresses + 7) / 8);
    }
    ~MIDI_Input_LED() {
      free (states);
    }
    void init() {
      pinMode(pin, OUTPUT);
    }
    bool update(uint8_t header, uint8_t data1, uint8_t data2) {
      uint8_t channel = (header & 0b1111) + 1;
      uint8_t address = data1;
      uint8_t addressIndex;
      uint8_t channelIndex;
      bool found = false;
      for (uint8_t i = 0; i < nb_channels; i++) {
        if (channel == channels[i]) {
          found = true;
          channelIndex = i;
          break;
        }
      }
      if (!found) {
        Serial.println("Channel not found");
        return false;
      }

      found = false;
      for (uint8_t i = 0; i < nb_addresses; i++) {
        if (address == addresses[i]) {
          found = true;
          addressIndex = i;
          break;
        }
      }
      if (!found) {
        Serial.println("Address not found");
        return false;
      }


      if (header == NOTE_OFF || (header == NOTE_ON && data2 == 0)) {
        states[(channelIndex + nb_channels * addressIndex) / 8] &= ~(1 << ((channelIndex + nb_channels * addressIndex) % 8));
      } else if (header == NOTE_ON) {
        states[(channelIndex + nb_channels * addressIndex) / 8] |= 1 << ((channelIndex + nb_channels * addressIndex) % 8);
      } else {
        Serial.println("Wrong message type");
        return true; // not a Note On or Note Off event, but channel and address match
      }
      refresh();
      return true;
    }
    void refresh() {
      digitalWrite(pin, states[(channelOffset + nb_channels * addressOffset) / 8] & (1 << ((channelOffset + nb_channels * addressOffset) % 8)));
    }

    void print() {
      Serial.print("Channels:\t");
      for (uint8_t i = 0; i < nb_channels; i++) {
        Serial.print(channels[i]); Serial.print(' ');
      }
      Serial.println();
      Serial.print("Addresses:\t");
      for (uint8_t i = 0; i < nb_addresses; i++) {
        Serial.print(addresses[i]); Serial.print(' ');
      }
      Serial.println();
    }
  private:
    pin_t pin;
    uint8_t *states = nullptr;
};

class MIDI_InputHandler {
  public:
    struct input_ll {
      input_ll* next = nullptr;
      MIDI_Input_Element* element;
    };
    void init() {
      for (input_ll *node = firstElement; node != nullptr; node = node->next) {
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
        Serial.printf("%02X %02X %02x\r\n", header, data1, data2, NOTE_ON);
        if (((header & 0b11110000) == NOTE_ON) || ((header & 0b11110000) == NOTE_OFF)) {
          for (input_ll *node = firstElement; node != nullptr; node = node->next) {
            if (node->element->update(header, data1, data2)) {
              Serial.println("Match");
              break;
            }
          }
        }
      } while (MIDI_In.available() > 0);
    };
    void add(MIDI_Input_LED& element) {
      add(&element);
    };
    void add(MIDI_Input_LED* element) {
      input_ll *newElement = new input_ll;
      newElement->element = element;
      newElement->next = nullptr;
      if (firstElement == nullptr)
      {
        firstElement = newElement;
        lastElement = firstElement;
      }
      else
      {

        lastElement->next = newElement;
        lastElement = newElement;
      }
    };
  private:
    input_ll* firstElement = nullptr, *lastElement = nullptr;

};

uint8_t ledChannels[] = {1};
uint8_t ledControllers[] = {  MUTE,
                              MUTE + 1,
                              MUTE + 2,
                              MUTE + 3,
                              MUTE + 4,
                              MUTE + 5,
                              MUTE + 6,
                              MUTE + 7
                           };
MIDI_Input_LED ledinput(14, ledControllers, ledChannels);
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

  ledinput.print();

  inputhandler.add(ledinput);
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
  ledinput.setAddressOffset(newSetting);
  ledinput.refresh();
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

