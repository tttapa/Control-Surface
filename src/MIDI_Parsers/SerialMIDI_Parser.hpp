#pragma once

#include "MIDI_Parser.hpp"
#include "SysExBuffer.hpp"

BEGIN_CS_NAMESPACE

class SerialMIDI_Parser : public MIDI_Parser {
  public:
    MIDI_read_t parse(uint8_t midibyte);

#if !IGNORE_SYSEX
    SysExMessage getSysEx() const override {
        return {sysexbuffer.getBuffer(), sysexbuffer.getLength(), 0};
    }
#endif

  protected:
#if !IGNORE_SYSEX
    SysExBuffer sysexbuffer;

    bool addSysExByte(uint8_t data) { return sysexbuffer.add(data); }
    void startSysEx() { sysexbuffer.start(); }
    void endSysEx() { sysexbuffer.end(); }
#endif

  private:
    bool thirdByte = false;
};

END_CS_NAMESPACE