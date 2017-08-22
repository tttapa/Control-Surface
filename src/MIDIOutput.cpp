#include "MIDIOutput.h"

void MIDIOutput::send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
{
    c--; // Channels are zero-based

    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0xF;        // bitmask low nibble
    d1 &= 0x7F;      // clear msb
    d2 &= 0x7F;      // clear msb

    sendImpl(m, c, d1, d2);
}

void MIDIOutput::send(uint8_t m, uint8_t c, uint8_t d1)
{
    c--; // Channels are zero-based

    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0xF;        // bitmask low nibble
    d1 &= 0x7F;      // clear msb

    sendImpl(m, c, d1);
}

MIDIOutput *MIDIOutput::DefaultMIDIOutput = nullptr;

void sendMIDI(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
{
    if (MIDIOutput::getDefault() != nullptr)
        MIDIOutput::getDefault()->send(m, c, d1, d2);
}
void sendMIDI(uint8_t m, uint8_t c, uint8_t d1)
{
    if (MIDIOutput::getDefault() != nullptr)
        MIDIOutput::getDefault()->send(m, c, d1);
}
void startMIDI()
{
    if (MIDIOutput::getDefault() != nullptr)
        MIDIOutput::getDefault()->begin();
}