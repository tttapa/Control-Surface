#pragma once

#include <AH/Arduino-Wrapper.h> // Stream
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Helper that pulls bytes out of an Arduino stream.
 * 
 * @ingroup MIDIParsers
 */
class StreamPuller {
  public:
    StreamPuller(Stream &stream) : stream(stream) {}

    /// Pull a byte out of the stream.
    /// @param[out] c
    ///             A new character from the stream (if available).
    /// @return True if a character was available, false otherwise.
    bool pull(uint8_t &c) {
        int read = stream.read();
        if (read >= 0)
            c = read;
        return read >= 0;
    }

    Stream &stream;
};

END_CS_NAMESPACE