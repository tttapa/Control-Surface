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
#else
#define USB_MIDI_NUMBER_OF_CABLES 1
#endif

BEGIN_CS_NAMESPACE

class USBMIDI_Parser : public MIDI_Parser {
  public:
    MIDIReadEvent parse(uint8_t *packet);

#if !IGNORE_SYSEX
    SysExMessage getSysExMessage() const {
        return {sysexbuffers[activeSysExCN].getBuffer(),
                sysexbuffers[activeSysExCN].getLength(), activeSysExCN};
    }
#endif

  protected:
    MIDIReadEvent handleChannelMessage(const uint8_t *packet, uint8_t cable);
    MIDIReadEvent handleSingleByte(const uint8_t *packet, uint8_t cable);
    MIDIReadEvent handleSysExStartContinue(const uint8_t *packet, uint8_t cable);
    MIDIReadEvent handleSysExEnd1B(const uint8_t *packet, uint8_t cable);
    template <uint8_t NumBytes>
    MIDIReadEvent handleSysExEnd(const uint8_t *packet, uint8_t cable);

  protected:
#if !IGNORE_SYSEX
    void startSysEx(uint8_t cable) { sysexbuffers[cable].start(); }
    void endSysEx(uint8_t cable) {
        sysexbuffers[cable].end();
        activeSysExCN = cable;
    }
    bool hasSysExSpace(uint8_t cable, uint8_t amount) const {
        return sysexbuffers[cable].hasSpaceLeft(amount);
    }
    void addSysExByte(uint8_t cable, uint8_t data) {
        sysexbuffers[cable].add(data);
    }
    void addSysExBytes(uint8_t cable, const uint8_t *data, uint8_t len) {
        sysexbuffers[cable].add(data, len);
    }
    bool receivingSysEx(uint8_t cable) const {
        return sysexbuffers[cable].isReceiving();
    }
#endif

    void storePacket(const uint8_t *packet) {
        memcpy(previousPacket, packet, 4);
    }

    uint8_t activeSysExCN = 0;

  private:
#if !IGNORE_SYSEX
    Array<SysExBuffer, USB_MIDI_NUMBER_OF_CABLES> sysexbuffers;
#endif
    uint8_t previousPacket[4] = {};
};

END_CS_NAMESPACE