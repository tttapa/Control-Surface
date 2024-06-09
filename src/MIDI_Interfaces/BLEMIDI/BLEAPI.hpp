#pragma once

/**
 * @file
 * Type definitions and callback interfaces for communication between the
 * low-level BLE stacks and higher-level MIDI BLE backends.
 */

#include <Settings/NamespaceSettings.hpp>

#include "Util/compat.hpp"

#include <cstddef>
#include <cstdint>

BEGIN_CS_NAMESPACE

/// Represents a handle to the connection to another device.
struct BLEConnectionHandle {
    uint16_t conn = 0xFFFF;
    explicit operator bool() const { return conn != 0xFFFF; }

#if __cplusplus < 201402L
    BLEConnectionHandle() = default;
    BLEConnectionHandle(uint16_t conn) : conn {conn} {}
#endif
};

/// Represents a handle to a local GATT characteristic.
struct BLECharacteristicHandle {
    uint16_t characteristic = 0xFFFF;
    explicit operator bool() const { return characteristic != 0xFFFF; }

#if __cplusplus < 201402L
    BLECharacteristicHandle() = default;
    BLECharacteristicHandle(uint16_t characteristic)
        : characteristic {characteristic} {}
#endif
};

/// Non-owning, std::span-style read-only view of BLE data.
struct BLEDataView {
    const uint8_t *data = nullptr;
    uint16_t length = 0;
    explicit operator bool() const { return length > 0; }

#if __cplusplus < 201402L
    BLEDataView() = default;
    BLEDataView(const uint8_t *data, uint16_t length)
        : data {data}, length {length} {}
#endif
};

/// Describes a byte buffer containing (part of) a BLE packet.
/// Packets can be stored across multiple buffers, in which case the first
/// first buffer has type `Packet` and subsequent buffers of the same packet
/// have the type `Continuation`.
enum class BLEDataType : uint8_t {
    None = 0,     ///< No buffers available.
    Packet,       ///< Buffer contains the start of a BLE packet.
    Continuation, ///< Buffer contains a chunk of a BLE packet.
};

/// Callable that returns the next chunk of data from a BLE packet when called.
/// Uses type erasure with a static buffer (no dynamic memory allocations).
class BLEDataGenerator {
  public:
    /// Get the next chunk of data from the BLE packet.
    /// Returns a chunk of size zero to indicate completion.
    /// @pre This wrapper is not empty.
    /// @pre There is still data available. Calling this function again after
    ///      the previous call returned an empty chunk is not allowed.
    BLEDataView operator()();
    /// Release the resources of the underlying data generator.
    void clear();
    /// Check if this wrapper contains an underlying data generator.
    explicit operator bool() const { return instance; }

    /// Create an empty BLEDataGenerator.
    BLEDataGenerator() = default;
    /// Store a callable of type @p T and initialize it by @p args.
    template <class T, class... Args>
    BLEDataGenerator(compat::in_place_type_t<T>, Args &&...args);
    /// Store a callable of type @p T (with cv qualifiers and references
    /// removed) and initialize it by forwarding @p t.
    template <class T>
    BLEDataGenerator(compat::in_place_t, T &&t);
    BLEDataGenerator(const BLEDataGenerator &) = delete;
    BLEDataGenerator &operator=(const BLEDataGenerator &) = delete;
    BLEDataGenerator(BLEDataGenerator &&other) noexcept;
    BLEDataGenerator &operator=(BLEDataGenerator &&other) noexcept;
    ~BLEDataGenerator() { clear(); }

  private:
    /// Type-erased interface.
    struct Iface;
    /// Specific class that implements the type-erased interface, wrapping the
    /// type @p T.
    template <class T>
    struct Impl;
    /// Alignment of the buffer to allocate the underlying data generator.
    using buffer_align_t = max_align_t;
    /// Size of the buffer to allocate the underlying data generator.
    static constexpr size_t capacity = 4 * sizeof(void *) - sizeof(Iface *);
    /// Buffer used for allocation of the underlying data generator.
    alignas(buffer_align_t) compat::byte storage[capacity];
    //// Type-erased pointer to the underlying data generator in @ref storage.
    Iface *instance = nullptr;
};

/// Should a buffer of BLEData be consumed immediately inside of the callback,
/// or can we hold on to it and process it later?
enum class BLEDataLifetime {
    /// Buffer is valid only during the callback. Do not keep any pointers to it.
    ConsumeImmediately,
    /// Buffer is valid for as long as the owning @ref BLEDataGenerator is not
    /// resumed or destroyed.
    Managed,
};

/// Defines the interface for callback functions registered by the low-level
/// BLE code.
/// @warning    These functions may be called from different tasks/threads or
///             low-priority interrupt handlers. You cannot take locks, and you
///             need to synchronize appropriately (e.g. using `std::atomic` or
///             by using critical sections).
class MIDIBLEInstance {
  public:
    virtual ~MIDIBLEInstance() = default;
    /// Called by the BLE stack when a connection is established.
    virtual void handleConnect(BLEConnectionHandle conn_handle) = 0;
    /// Called by the BLE stack when a connection is terminated.
    virtual void handleDisconnect(BLEConnectionHandle conn_handle) = 0;
    /// Called by the BLE stack when the maximum transmission unit for the
    /// connection changes.
    virtual void handleMTU(BLEConnectionHandle conn_handle, uint16_t mtu) = 0;
    /// Called by the BLE stack when the central subscribes to receive
    /// notifications for the MIDI GATT characteristic.
    virtual void handleSubscribe(BLEConnectionHandle conn_handle,
                                 BLECharacteristicHandle char_handle,
                                 bool notify) = 0;
    /// Called by the BLE stack when the central writes data to the MIDI GATT
    /// characteristic.
    virtual void handleData(BLEConnectionHandle conn_handle,
                            BLEDataGenerator &&data,
                            BLEDataLifetime lifetime) = 0;
};

/// Configuration options for the low-level BLE code.
struct BLESettings {
    /// Device name (used for advertising)
    const char *device_name = "Control Surface MIDI";
    /// Connection intervals as multiples of 1.25 milliseconds
    /// (e.g.0x000C = 15 ms).
    struct {
        uint16_t minimum = 0x000C;
        uint16_t maximum = 0x000C;
    } connection_interval {};
    /// Set to true if you want the Arduino to always initiate the Bluetooth
    /// bonding or secure connection. As a result, it will show up as a "paired"
    /// device on your computer/phone/tablet. If set to false, security is still
    /// supported, but the central device should take the initiative.
    bool initiate_security = false;
};

END_CS_NAMESPACE

#include "BLEAPI.ipp"
