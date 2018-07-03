#pragma once

#include "MIDI_Parser.h"

class SerialMIDI_Parser : public MIDI_Parser {
  public:
    MIDI_read_t parse(uint8_t midibyte);

  private:
    bool thirdByte = false;
};