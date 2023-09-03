#include "MIDI_Parser.hpp"
#include "SysExBuffer.hpp"
#include <AH/Containers/Array.hpp>

#ifdef MIDI_NUM_CABLES
#define USB_MIDI_NUMBER_OF_CABLES MIDI_NUM_CABLES
#elif defined(USB_MIDI4_SERIAL) || defined(USB_MIDI4)
#define USB_MIDI_NUMBER_OF_CABLES 4
#elif defined(USB_MIDI16_AUDIO_SERIAL) || defined(USB_MIDI16_SERIAL) ||        \
    defined(USB_MIDI16)
// TODO: || defined(USB_EVERYTHING)
#define USB_MIDI_NUMBER_OF_CABLES 16
#elif !defined(ARDUINO) || defined(DOXYGEN)
#define USB_MIDI_NUMBER_OF_CABLES 16
#else
#define USB_MIDI_NUMBER_OF_CABLES 1
#endif

BEGIN_CS_NAMESPACE

/**
 * @brief   Parser for MIDI over USB packets.
 * 
 * @ingroup MIDIParsers
 */
class USBMIDI_Parser : public MIDI_Parser {
  public:
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;

    /**
     * @brief   Parse one incoming MIDI message.
     * @param   puller
     *          The source of MIDI USB packets.
     * @return  The type of MIDI message available, or 
     *          `MIDIReadEvent::NO_MESSAGE` if `puller` ran out of packets
     *          before a complete message was parsed.
     */
    template <class BytePuller>
    MIDIReadEvent pull(BytePuller &&puller);

  protected:
    /// Feed a new packet to the parser.
    MIDIReadEvent feed(MIDIUSBPacket_t packet);
    /// Resume the parser with the previously stored and unhandled packet.
    MIDIReadEvent resume();

  public:
#if !IGNORE_SYSEX
    /// Get the latest SysEx message.
    SysExMessage getSysExMessage() const {
        return {
            sysexbuffers[activeCable.getRaw()].getBuffer(),
            sysexbuffers[activeCable.getRaw()].getLength(),
            activeCable,
        };
    }
#endif

  protected:
    MIDIReadEvent handleChannelMessage(MIDIUSBPacket_t packet, Cable cable);
    MIDIReadEvent handleSingleByte(MIDIUSBPacket_t packet, Cable cable);
    MIDIReadEvent handleSysExStartCont(MIDIUSBPacket_t packet, Cable cable);
    template <uint8_t NumBytes>
    MIDIReadEvent handleSysExEnd(MIDIUSBPacket_t packet, Cable cable);
    MIDIReadEvent handleSysCommon(MIDIUSBPacket_t packet, Cable cable);

  protected:
#if !IGNORE_SYSEX
    void startSysEx(Cable cable) { sysexbuffers[cable.getRaw()].start(); }
    void endSysEx(Cable cable) {
        sysexbuffers[cable.getRaw()].end();
        activeCable = cable;
    }
    void endSysExChunk(Cable cable) { activeCable = cable; }
    bool hasSysExSpace(Cable cable, uint8_t amount) const {
        return sysexbuffers[cable.getRaw()].hasSpaceLeft(amount);
    }
    void addSysExByte(Cable cable, uint8_t data) {
        sysexbuffers[cable.getRaw()].add(data);
    }
    void addSysExBytes(Cable cable, const uint8_t *data, uint8_t len) {
        sysexbuffers[cable.getRaw()].add(data, len);
    }
    bool receivingSysEx(Cable cable) const {
        return sysexbuffers[cable.getRaw()].isReceiving();
    }

    void storePacket(MIDIUSBPacket_t packet) { storedPacket = packet; }
    bool hasStoredPacket() const { return storedPacket[0] != 0x00; }
    MIDIUSBPacket_t popStoredPacket() {
        MIDIUSBPacket_t t = storedPacket;
        storedPacket[0] = 0x00;
        return t;
    }

    Cable activeCable = Cable_1;

  private:
    SysExBuffer sysexbuffers[USB_MIDI_NUMBER_OF_CABLES] = {};
    MIDIUSBPacket_t storedPacket = {{ 0x00 }};
#endif
};

template <class BytePuller>
inline MIDIReadEvent USBMIDI_Parser::pull(BytePuller &&puller) {
    // First try resuming the parser, we might have a stored packet that has to
    // be parsed first.
    MIDIReadEvent evt = resume();
    if (evt != MIDIReadEvent::NO_MESSAGE)
        return evt;

    // If resumption didn't produce a message, read new packets from the input
    // and parse them until either we get a message, or until the input runs out
    // of new packets.
    MIDIUSBPacket_t midiPacket;
    while (puller.pull(midiPacket)) {
        evt = feed(midiPacket);
        if (evt != MIDIReadEvent::NO_MESSAGE)
            return evt;
    }
    return MIDIReadEvent::NO_MESSAGE;
}

END_CS_NAMESPACE