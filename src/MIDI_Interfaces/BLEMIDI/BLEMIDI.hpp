#pragma once

#ifdef ARDUINO

#ifndef ARDUINO_ARCH_ESP32
#error "MIDI over Bluetooth is only supported on ESP32 boards"
#endif

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <AH/Error/Error.hpp>

BEGIN_CS_NAMESPACE

/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
constexpr static const char *BLE_MIDI_SERVICE_UUID
    = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
constexpr static const char *BLE_MIDI_CHARACTERISTIC_UUID
    = "7772e5db-3868-4112-a1a9-f2669d106bf3";

/// Name of Bluetooth LE MIDI device.
constexpr static const char *BLE_MIDI_NAME = "Control Surface (BLE)";

/// Handles the Bluetooth Low Energy BLE server, characteristics, callbacks 
/// etc., but not the actual MIDI stuff.  
/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
class BLEMIDI {
  public:
    ~BLEMIDI() = default; // TODO

    /// @note   Must be called after begin.
    void setServerCallbacks(BLEServerCallbacks *cb);
    /// @note   Must be called after begin.
    void setCharacteristicsCallbacks(BLECharacteristicCallbacks *cb);

    /// Initialize.
    void begin(BLEServerCallbacks *serverCallbacks,
               BLECharacteristicCallbacks *midiCallbacks);

    /**
     * @brief   Send the given BLE MIDI packet.
     * 
     * @param   data
     *          Pointer to the data.
     * @param   len
     *          Length of the data in bytes. Maximum length is the MTU minus 
     *          three bytes.
     */
    void notifyValue(const uint8_t *data, size_t len);

    /**
     * @brief Get the received BLE MIDI packet.
     */
    std::string getValue() const;

    /**
     * @brief   Get the minimum MTU of all connected clients.
     */
    uint16_t getMinMTU() const;

  private:
    BLECharacteristic *pCharacteristic = nullptr;
    BLEService *pService = nullptr;
    BLEServer *pServer = nullptr;
    BLE2902 *pDescriptor = nullptr;

  private:
    void cleanup();
};

END_CS_NAMESPACE

#else

#include "BLEMIDI-Mock.ipp"

#endif