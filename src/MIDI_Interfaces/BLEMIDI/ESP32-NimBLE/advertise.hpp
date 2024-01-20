#pragma once

#include <cstdint>

namespace cs::midi_ble_nimble {

void advertise(uint8_t addr_type);
void set_advertise_connection_interval(uint16_t min_itvl, uint16_t max_itvl);

} // namespace cs::midi_ble_nimble

#include "advertise.ipp"