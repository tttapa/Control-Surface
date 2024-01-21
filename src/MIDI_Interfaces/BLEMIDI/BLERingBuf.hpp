#pragma once

#include <cstdint>
#include <cstring>

#include <Settings/NamespaceSettings.hpp>

#include <MIDI_Interfaces/BLEMIDI/BLEAPI.hpp>

BEGIN_CS_NAMESPACE

template <class T>
struct NonatomicBLERingBufSize {
    T value;
    /// Alignment for size, and read/write pointers to avoid false sharing.
    constexpr static size_t alignment = alignof(T);
    T load_acquire() const { return value; }
    void add_release(T t) { value += t; }
    void sub_release(T t) { value -= t; }
};

/// Circular FIFO buffer for buffering BLE packet data. It supports both
/// complete BLE packets and packets split over multiple chunks. Full packets
/// that are added to the FIFO might be split up over multiple chunks.
/// @tparam Capacity
///         Buffer size (bytes). Note that the actual maximum data size may be
///         up to 6 bytes less because of data structure overhead.
/// @tparam SizeT
///         The type to use for the size of tbe buffer. Should be atomic if this
///         buffer is to be used as a SPSC queue between two threads.
///         See @ref NonatomicBLERingBufSize for an example.
template <uint_fast16_t Capacity,
          class SizeT = NonatomicBLERingBufSize<uint_fast16_t>>
class BLERingBuf {
  private:
    struct Header {
        uint16_t size : 14;
        uint8_t type : 2;
        Header() = default;
        Header(uint16_t size, BLEDataType type)
            : size {size}, type {static_cast<uint8_t>(type)} {}
        BLEDataType getType() const { return static_cast<BLEDataType>(type); }
    };
    constexpr static uint_fast16_t header_size = sizeof(Header);
    static_assert(header_size == 2, "");

    constexpr static uint_fast16_t capacity = Capacity;
    unsigned char buffer[capacity];
    alignas(SizeT::alignment) uint_fast16_t read_p = 0;
    alignas(SizeT::alignment) uint_fast16_t write_p = header_size;
    alignas(SizeT::alignment) SizeT size {header_size};

    constexpr static uint_fast16_t ceil_h(uint_fast16_t i) {
        return ((i + header_size - 1) / header_size) * header_size;
    }

  public:
    BLERingBuf() {
        Header header {0, BLEDataType::None};
        static_assert(capacity % header_size == 0, "");
        std::memcpy(buffer, &header, header_size);
    }

    /// Copy the given data into the buffer. May be split up into two chunks,
    /// in which case the type will be set to @ref BLEDataType::Continuation
    /// for the second chunk.
    /// @retval false   The buffer is full, nothing added to the buffer.
    bool push(BLEDataView data, BLEDataType type = BLEDataType::Packet) {
        if (type == BLEDataType::None)
            return false;
        uint_fast16_t loc_size = size.load_acquire();
        uint_fast16_t add_size = 0;
        assert(loc_size <= capacity);
        assert(write_p < capacity);
        assert(write_p % header_size == 0);
        // We need to write at least one header
        uint_fast16_t expected_req_size = loc_size + data.length + header_size;
        if (expected_req_size > capacity)
            return false; // not enough space
        // Contiguous size remaining (excluding header)
        uint_fast16_t contig_size = capacity - write_p - header_size;
        assert(contig_size < capacity);
        // We may need to write a second header
        uint_fast16_t worst_case_req_size = expected_req_size + header_size;
        if (data.length > contig_size && worst_case_req_size > capacity)
            return false; // not enough space
        // Write the first header for the packet
        uint16_t size_1 = std::min<uint_fast16_t>(contig_size, data.length);
        Header header {size_1, type};
        std::memcpy(buffer + write_p, &header, sizeof(header));
        write_p += header_size;
        add_size += header_size;
        // Write first data
        if (size_1 > 0) // avoid memcpy with nullptr
            std::memcpy(buffer + write_p, data.data, size_1);
        write_p += ceil_h(size_1);
        add_size += ceil_h(size_1);
        if (write_p == capacity)
            write_p = 0;
        // Now write the remainder at the beginning of the circular buffer
        uint16_t size_2 = data.length - size_1;
        if (size_2 > 0) {
            // Write the continuation header
            Header header {size_2, BLEDataType::Continuation};
            std::memcpy(buffer + write_p, &header, sizeof(header));
            write_p += header_size;
            add_size += header_size;
            // Write the remainder of the data
            std::memcpy(buffer + write_p, data.data + size_1, size_2);
            write_p += ceil_h(size_2);
            add_size += ceil_h(size_2);
        }
        size.add_release(add_size);
        return true;
    }

    /// Get a view to the next chunk of data. The view remains valid until the
    /// next call to @ref pop.
    /// @retval BLEDataType::None
    ///         No data available.
    /// @retval BLEDataType::Packet
    ///         The @p data output parameter points to the first chunk of a
    ///         packet.
    /// @retval BLEDataType::Continuation
    ///         The @p data output parameter points to a chunk of continuation
    ///         data of the same packet.
    BLEDataType pop(BLEDataView &data) {
        uint_fast16_t loc_size = size.load_acquire();
        assert(loc_size >= header_size);
        assert(read_p < capacity);
        assert(read_p % header_size == 0);
        // Read the old header
        Header old_header;
        std::memcpy(&old_header, buffer + read_p, sizeof(old_header));
        // If the previous chunk is the only thing left in the buffer
        if (loc_size - header_size == ceil_h(old_header.size)) {
            // If there is still actual data left in the buffer
            if (old_header.getType() != BLEDataType::None) {
                // Free the old data, preserving space for a header
                read_p += ceil_h(old_header.size);
                size.sub_release(ceil_h(old_header.size));
                // Write an empty header
                Header header {0, BLEDataType::None};
                std::memcpy(buffer + read_p, &header, sizeof(header));
            }
            data = {nullptr, 0};
            return BLEDataType::None;
        } else {
            // Otherwise, discard the old data and header
            read_p += header_size + ceil_h(old_header.size);
            size.sub_release(header_size + ceil_h(old_header.size));
            if (read_p == capacity)
                read_p = 0;
            // Read the next header
            Header header;
            std::memcpy(&header, buffer + read_p, sizeof(header));
            data = {buffer + read_p + header_size, header.size};
            return header.getType();
        }
    }
};

END_CS_NAMESPACE
