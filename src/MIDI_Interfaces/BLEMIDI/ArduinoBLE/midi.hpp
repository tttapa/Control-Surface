#pragma once

#include <MIDI_Interfaces/BLEMIDI/BLEAPI.hpp>

BEGIN_CS_NAMESPACE

namespace arduino_ble_midi {

bool init(MIDIBLEInstance &instance, BLESettings ble_settings);
void poll();
void notify(BLEDataView data);

} // namespace arduino_ble_midi

END_CS_NAMESPACE

// We cannot do this in a separate .cpp file, because the user might not have
// the ArduinoBLE library installed, and the Arduino library dependency scanner
// does not support __has_include.
#include "midi.ipp"
