#pragma once

#include <Settings/NamespaceSettings.hpp>

#include <cstddef>
#include <cstdint>
#include <utility>

BEGIN_CS_NAMESPACE

/// Represents a handle to the connection to another device.
struct BLEConnectionHandle {
    uint16_t conn = 0xFFFF;
    explicit operator bool() const {return conn != 0xFFFF; }
};

/// Represents a handle to a local GATT characteristic.
struct BLECharacteristicHandle {
    uint16_t characteristic = 0xFFFF;
    explicit operator bool() const {return characteristic != 0xFFFF; }
};

/// Span-style read-only view of BLE data.
struct BLEDataView {
    const uint8_t *data = nullptr;
    uint16_t length = 0;
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
    template <class T, class... Args>
    BLEDataGenerator(std::in_place_type_t<T>, Args &&...args);
    template <class T>
    BLEDataGenerator(std::in_place_t, T &&t);
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
    static constexpr size_t buffer_size = 15 * sizeof(void *);
    /// Buffer used for allocation of the underlying data generator.
    alignas(buffer_align_t) std::byte storage[buffer_size];
    //// Type-erased pointer to the underlying data generator.
    Iface *instance = nullptr;
};

enum class BLEDataLifetime {
    ConsumeImmediately,
    Managed,
};

class MIDIBLEInstance {
  public:
    virtual void handleConnect(BLEConnectionHandle conn_handle) = 0;
    virtual void handleDisconnect(BLEConnectionHandle conn_handle) = 0;
    virtual void handleMTU(BLEConnectionHandle conn_handle, uint16_t mtu) = 0;
    virtual void handleSubscribe(BLEConnectionHandle conn_handle,
                                 BLECharacteristicHandle char_handle,
                                 bool notify) = 0;
    virtual void handleData(BLEConnectionHandle conn_handle,
                            BLEDataGenerator &&data,
                            BLEDataLifetime lifetime) = 0;
};

struct BLESettings {
    const char *device_name;
};

END_CS_NAMESPACE

#include "BLEAPI.ipp"
