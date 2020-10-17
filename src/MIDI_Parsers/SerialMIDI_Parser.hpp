#pragma once

#include "MIDI_Parser.hpp"
#include "SysExBuffer.hpp"

BEGIN_CS_NAMESPACE

/**
 * @brief   Parser for Serial MIDI streams.
 * 
 * @ingroup MIDIParsers
 */
class SerialMIDI_Parser : public MIDI_Parser {
  public:
    /**
     * @brief   Parse one incoming MIDI message.
     * @param   puller
     *          The source of MIDI bytes.
     * @return  The type of MIDI message available, or 
     *          `MIDIReadEvent::NO_MESSAGE` if `puller` ran out of bytes before
     *          a complete message was parsed.
     */
    template <class BytePuller>
    MIDIReadEvent pull(BytePuller &&puller);

  protected:
    /// Feed a new byte to the parser.
    MIDIReadEvent feed(uint8_t midibyte);
    /// Resume the parser with the previously stored and unhandled byte.
    MIDIReadEvent resume();

#if !IGNORE_SYSEX
  public:
    /// Get the latest SysEx message.
    SysExMessage getSysExMessage() const {
        return {sysexbuffer.getBuffer(), sysexbuffer.getLength()};
    }

  protected:
    void addSysExByte(uint8_t data) { sysexbuffer.add(data); }
    bool hasSysExSpace() const { return sysexbuffer.hasSpaceLeft(); }
    void startSysEx() { sysexbuffer.start(); }
    void endSysEx() { sysexbuffer.end(); }

    SysExBuffer sysexbuffer;
#endif

  protected:
    MIDIReadEvent handleRealTime(uint8_t midiByte);
    MIDIReadEvent handleStatus(uint8_t midiByte);
    MIDIReadEvent handleData(uint8_t midiByte);

  protected:
    /// Store a byte to parse later. This is used when the SysEx buffer is full,
    /// for example. The byte cannot be added to the buffer now, so store it to
    /// add it the next time the parser is updated.
    void storeByte(uint8_t midiByte) { storedByte = midiByte; }
    /// If a byte was been stored previously. If this is the case, you should
    /// parse this byte first before reading a new byte.
    bool hasStoredByte() const { return storedByte != 0xFF; }
    /// Get the stored byte. Afterwards, @ref hasStoredByte will return false.
    uint8_t popStoredByte() {
        uint8_t t = storedByte;
        storedByte = 0xFF;
        return t;
    }

  private:
    /// Flag that remembers that the next data byte will be the third byte of
    /// a message.
    bool thirdByte = false;
    /// @see @ref storeByte
    uint8_t storedByte = 0xFF;
};

template <class BytePuller>
inline MIDIReadEvent SerialMIDI_Parser::pull(BytePuller &&puller) {
    // First try resuming the parser, we might have a stored byte that has to
    // be parsed first.
    MIDIReadEvent evt = resume();
    if (evt != MIDIReadEvent::NO_MESSAGE)
        return evt;

    // If resumption didn't produce a message, read new bytes from the input and
    // parse them until either we get a message, or until the input runs out of
    // new bytes.
    uint8_t midiByte;
    while (puller.pull(midiByte)) {
        evt = feed(midiByte);
        if (evt != MIDIReadEvent::NO_MESSAGE)
            return evt;
    }
    return MIDIReadEvent::NO_MESSAGE;
}

END_CS_NAMESPACE