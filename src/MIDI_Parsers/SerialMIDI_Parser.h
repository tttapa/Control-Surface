#ifndef SERIALMIDI_PARSER_H
#define SERIALMIDI_PARSER_H

#include "MIDI_Parser.h"

class SerialMIDI_Parser : public MIDI_Parser
{
  public:
    MIDI_read_t parse(uint8_t midibyte);

  private:
    bool thirdByte = false;
};

#endif // SERIALMIDI_PARSER_H