#include "PBOut.h"
#include "../../Control_Surface/Control_Surface_Class.h"

PBOut::PBOut(uint8_t channel) : channel(channel) {}

void PBOut::update() { refresh(); }

void PBOut::send(uint16_t value) {
    Control_Surface.MIDI()->send(PITCH_BEND,
                                 channel + channelOffset * tracksPerBank,
                                 value & 0x7F, value >> 7);
}