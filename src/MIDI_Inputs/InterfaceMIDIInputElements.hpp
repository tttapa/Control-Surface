#pragma once

#include <AH/STL/cstdint>
#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

namespace Interfaces {

/// Abstract interface for MIDI input elements that receive and store a 7-bit
/// value.
class IValue {
  public:
    /// @name   Detecting changes
    /// @{

    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

    /// @}

    virtual uint8_t getValue() const = 0;

  protected:
    bool dirty = true;
};

/// Abstract interface for MIDI input elements that receive and store a 14-bit
/// value.
class IValue14 {
  public:
    /// @name   Detecting changes
    /// @{

    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

    /// @}

    virtual uint16_t getValue() const = 0;

  protected:
    bool dirty = true;
};

namespace MCU {

class IVPot {
  public:
    /// @name   Detecting changes
    /// @{

    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

    /// @}

    virtual bool getCenterLed() const = 0;
    virtual uint8_t getStartOn() const = 0;
    virtual uint8_t getStartOff() const = 0;

  protected:
    bool dirty = true;
};

/** 
 * @brief   An abstract interface for VU meters. To allow for both floating 
 *          point values and integers, all values are integers under the hood.
 * 
 * Using floats instead integers would be a strange choice as LED bar VU meters
 * have discrete levels.  
 * Continuous "analog" VU meters can use or override the `getFloatValue()` 
 * method.
 */
class IVU {
  public:
    IVU(uint8_t max, bool alwaysDirty = false)
        : max(max), alwaysDirty(alwaysDirty) {}

    /// @name   Detecting changes
    /// @{

    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = alwaysDirty; }

    /// @}

    /// Return the VU meter value as an integer.
    virtual uint8_t getValue() = 0;
    /// Return the overload status.
    virtual bool getOverload() = 0;
    /// Get the VU meter value as a floating point number.
    virtual float getFloatValue() { return (float)getValue() / getMax(); }
    /// Get the maximum value that this VU meter can return.
    uint8_t getMax() const { return max; }

  protected:
    uint8_t max;
    bool alwaysDirty;
    bool dirty = true;
};

} // namespace MCU

} // namespace Interfaces

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
