#pragma once

#include <cstdint>
#include <cstring>

#include <Settings/NamespaceSettings.hpp>

#include <MIDI_Interfaces/BLEMIDI/BLEAPI.hpp>

BEGIN_CS_NAMESPACE

/// Circular FIFO buffer for buffering BLE packet data. It supports both
/// complete BLE packets and packets split over multiple chunks. Full packets
/// that are added to the FIFO might be split up over multiple chunks.
template <uint16_t Capacity>
class BLERingBuf {
  private:
    constexpr static uint16_t capacity = Capacity;
    unsigned char buffer[capacity];
    uint16_t read_p = 0;
    uint16_t write_p = 2;
    uint16_t size = 2;

    constexpr static uint16_t ceil2(uint16_t i) { return ((i + 1) / 2) * 2; }

    struct Header {
        uint16_t size : 14;
        BLEDataType type : 2;
    };

  public:
    BLERingBuf() {
        Header header {0, BLEDataType::None};
        static_assert(capacity % 2 == 0, "");
        static_assert(sizeof(header) == 2, "");
        std::memcpy(buffer, &header, 2);
    }

    /// Copy the given data into the buffer. May be split up into two chunks,
    /// in which case the type will be set to @ref BLEDataType::Continuation
    /// for the second chunk.
    /// @retval false   The buffer is full, nothing added to the buffer.
    bool push(BLEDataView data, BLEDataType type = BLEDataType::Packet) {
        assert(size <= capacity);
        assert(write_p < capacity);
        assert(write_p % 2 == 0);
        // We need to write at least one header
        if (size + data.length + 2 > capacity)
            return false; // not enough space
        // Contiguous size remaining (excluding header)
        uint16_t contig_size = capacity - write_p - 2;
        assert(contig_size < capacity);
        // We may need to write a second header
        if (data.length > contig_size && size + data.length + 4 > capacity)
            return false; // not enough space
        // Write the first header for the packet
        uint16_t size_1 = std::min(contig_size, data.length);
        Header header {size_1, type};
        std::memcpy(buffer + write_p, &header, 2);
        write_p += 2;
        size += 2;
        // Write first data
        if (size_1 > 0) // avoid memcpy with nullptr
            std::memcpy(buffer + write_p, data.data, size_1);
        write_p += ceil2(size_1);
        size += ceil2(size_1);
        if (write_p == capacity)
            write_p = 0;
        // Now write the remainder at the beginning of the circular buffer
        uint16_t size_2 = data.length - size_1;
        if (size_2 > 0) {
            // Write the continuation header
            Header header {size_2, BLEDataType::Continuation};
            std::memcpy(buffer + write_p, &header, 2);
            write_p += 2;
            size += 2;
            // Write the remainder of the data
            std::memcpy(buffer + write_p, data.data + size_1, size_2);
            size += ceil2(size_2);
            write_p += ceil2(size_2);
        }
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
        assert(size >= 2);
        assert(read_p < capacity);
        assert(read_p % 2 == 0);
        // Read the old header
        Header old_header;
        std::memcpy(&old_header, buffer + read_p, 2);
        // If the old data is the only thing left in the buffer
        if (size - 2 == ceil2(old_header.size)) {
            // Clean up the old data, preserving space for a header
            read_p += ceil2(old_header.size);
            size -= ceil2(old_header.size);
            // Write an empty header
            Header empty_header {0, BLEDataType::None};
            std::memcpy(buffer + read_p, &empty_header, 2);
            data = {nullptr, 0};
            return BLEDataType::None;
        } else {
            // Otherwise, discard the old data
            read_p += 2 + ceil2(old_header.size);
            size -= 2 + ceil2(old_header.size);
            if (read_p == capacity)
                read_p = 0;
            // Read the next header
            Header header;
            std::memcpy(&header, buffer + read_p, 2);
            data = {buffer + read_p + 2, header.size};
            return header.type;
        }
    }
};

END_CS_NAMESPACE
