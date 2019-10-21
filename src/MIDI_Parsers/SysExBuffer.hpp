#pragma once

#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

class SysExBuffer {
  private:
    uint8_t SysExBuffer[SYSEX_BUFFER_SIZE];
    size_t SysExLength = 0;
    bool receiving = false;

  public:
    /// Start a new SysEx message.
    void start();
    /// Finish the current SysEx message.
    void end();
    /// Add a byte to the current SysEx message.
    bool add(uint8_t data);
    /// Check if the buffer has at least 1 byte of free space available.
    bool hasSpaceLeft() const;
    /// Check if the buffer is receiving a SysEx message.
    bool isReceiving() const;
    /// Get a pointer to the buffer.
    const uint8_t *getBuffer() const;
    /// Get the length of the SysEx message in the buffer.
    size_t getLength() const;
};

END_CS_NAMESPACE