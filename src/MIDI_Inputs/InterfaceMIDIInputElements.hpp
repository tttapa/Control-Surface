#pragma once

#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>

AH_DIAGNOSTIC_WERROR()

#include <MIDI_Inputs/MCU/VPotRing.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

BEGIN_CS_NAMESPACE

namespace Interfaces {

/// Abstract interface for MIDI input elements that receive and store a 7-bit
/// value.
class IValue {
  public:
    virtual bool getDirty() const = 0;
    virtual void clearDirty() = 0;
    virtual uint8_t getValue() const = 0;
};

/// @see    NoteCCKPValue
/// @see    IValue
template <MIDIMessageType Type>
class INoteCCKPValue : public NoteCCKPValue<Type>, public IValue {
  public:
    using NoteCCKPValue<Type>::NoteCCKPValue;

    bool getDirty() const override { return NoteCCKPValue<Type>::getDirty(); }
    void clearDirty() override { NoteCCKPValue<Type>::clearDirty(); }
    uint8_t getValue() const override {
        return NoteCCKPValue<Type>::getValue();
    }
};

/// Class that listens for MIDI Note events on a single address and saves their
/// value.
/// @ingroup    MIDIInputElements
using NoteValue = INoteCCKPValue<MIDIMessageType::NOTE_ON>;

/// Class that listens for MIDI Control Change events on a single address and
/// saves their value.
/// @ingroup    MIDIInputElements
using CCValue = INoteCCKPValue<MIDIMessageType::CONTROL_CHANGE>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// saves their value.
/// @ingroup    MIDIInputElements
using KPValue = INoteCCKPValue<MIDIMessageType::KEY_PRESSURE>;

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

namespace MCU {

class IVPot {
  public:
    virtual bool getDirty() const = 0;
    virtual void clearDirty() = 0;
    virtual uint8_t getStartOn() const = 0;
    virtual uint8_t getStartOff() const = 0;
    virtual bool getCenterLed() const = 0;
};

class VPotRing : public CS_NAMESPACE_NAME::MCU::VPotRing, public IVPot {
  public:
    using CS_NAMESPACE_NAME::MCU::VPotRing::VPotRing;

    bool getDirty() const override {
        return CS_NAMESPACE_NAME::MCU::VPotRing::getDirty();
    }
    void clearDirty() override {
        CS_NAMESPACE_NAME::MCU::VPotRing::clearDirty();
    }
    uint8_t getStartOn() const override {
        return CS_NAMESPACE_NAME::MCU::VPotRing::getStartOn();
    }
    uint8_t getStartOff() const override {
        return CS_NAMESPACE_NAME::MCU::VPotRing::getStartOff();
    }
    bool getCenterLed() const override {
        return CS_NAMESPACE_NAME::MCU::VPotRing::getCenterLed();
    }
};

} // namespace MCU

} // namespace Interfaces

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
