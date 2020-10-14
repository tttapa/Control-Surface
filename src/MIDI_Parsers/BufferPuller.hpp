#pragma once

#include <AH/STL/vector>        // std::vector
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Helper to pull bytes out of a buffer.
 * @ingroup MIDIParsers
 */
class BufferPuller {
  public:
    BufferPuller(const uint8_t *buffer, size_t length)
        : buffer(buffer), end(buffer + length) {}

    BufferPuller(const std::vector<uint8_t> &buffer)
        : BufferPuller(buffer.data(), buffer.size()) {}

    template <size_t N>
    BufferPuller(const uint8_t (&buffer)[N]) : BufferPuller(buffer, N) {}

    /// Pull a byte out of the buffer.
    /// @param[out] c
    ///             A new byte from the buffer (if available).
    /// @return True if a byte was available, false otherwise.
    bool pull(uint8_t &c) {
        if (buffer != end) {
            c = *buffer++;
            return true;
        }
        return false;
    }

  private:
    const uint8_t *buffer;
    const uint8_t *const end;
};

END_CS_NAMESPACE