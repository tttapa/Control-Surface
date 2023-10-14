#pragma once

#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Helper for storing the System Exclusive messages being received by
 *          a MIDI parser.
 * 
 * @ingroup MIDIParsers
 */
class SysExBuffer {
  private:
    uint8_t buffer[SYSEX_BUFFER_SIZE];
    uint16_t length = 0;
    bool receiving = false;

  public:
    /// Start a new SysEx message.
    void start();
    /// Finish the current SysEx message.
    void end();
    /// Add a byte to the current SysEx message.
    void add(uint8_t data);
    /// Add multiple bytes to the current SysEx message.
    void add(const uint8_t *data, uint8_t len);
    /// Check if the buffer has at least `amount` bytes of free space available.
    bool hasSpaceLeft(uint8_t amount = 1) const;
    /// Check if the buffer is receiving a SysEx message.
    bool isReceiving() const;
    /// Get a pointer to the buffer.
    const uint8_t *getBuffer() const;
    /// Get the length of the SysEx message in the buffer.
    uint16_t getLength() const;
};

END_CS_NAMESPACE