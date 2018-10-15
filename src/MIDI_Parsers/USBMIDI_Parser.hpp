#include "MIDI_Parser.hpp"

class USBMIDI_Parser : public MIDI_Parser {
  public:
    MIDI_read_t parse(uint8_t *packet);
};
