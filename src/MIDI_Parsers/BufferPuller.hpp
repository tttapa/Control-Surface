#pragma once

#include <AH/STL/vector> // std::vector
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Helper to pull bytes or other objects out of a buffer.
 * @ingroup MIDIParsers
 */
template <class T = uint8_t>
class BufferPuller_ {
  public:
    BufferPuller_(const T *buffer, size_t length)
        : buffer(buffer), end(buffer + length) {}

    /// Pull a value out of the buffer.
    /// @param[out] output
    ///             A new value from the buffer (if available).
    /// @return True if a value was available, false otherwise.
    bool pull(T &output) {
        if (buffer != end) {
            output = *buffer++;
            return true;
        }
        return false;
    }

  private:
    const T *buffer;
    const T *const end;
};

template <class T>
BufferPuller_<T> BufferPuller(const T *buffer, size_t length) {
    return {buffer, length};
}
template <class T>
BufferPuller_<T> BufferPuller(const std::vector<T> &buffer) {
    return {buffer.data(), buffer.size()};
}
template <class T, size_t N>
BufferPuller_<T> BufferPuller(const T (&buffer)[N]) {
    return {buffer, N};
}

END_CS_NAMESPACE