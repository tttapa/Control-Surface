#include "MIDI_Interface.hpp"
#include "MIDI_Callbacks.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// Managing the default MIDI interface

MIDI_Interface::~MIDI_Interface() {
    if (getDefault() == this)
        DefaultMIDI_Interface = nullptr;
}

void MIDI_Interface::setAsDefault() { DefaultMIDI_Interface = this; }

#ifndef ARDUINO
#define DOWN_CAST dynamic_cast
#else
#define DOWN_CAST static_cast
#endif

MIDI_Interface *MIDI_Interface::getDefault() {
    return DefaultMIDI_Interface == nullptr
               ? DOWN_CAST<MIDI_Interface *>(updatables.getLast())
               : DefaultMIDI_Interface;
}

MIDI_Interface *MIDI_Interface::DefaultMIDI_Interface = nullptr;

// -------------------------------------------------------------------------- //

// Handling incoming MIDI events

void MIDI_Interface::onChannelMessage(ChannelMessage message) {
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onChannelMessage(*this, message);
}

void MIDI_Interface::onSysExMessage(SysExMessage message) {
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onSysExMessage(*this, message);
}

void MIDI_Interface::onSysCommonMessage(SysCommonMessage message) {
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onSysCommonMessage(*this, message);
}

void MIDI_Interface::onRealTimeMessage(RealTimeMessage message) {
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onRealTimeMessage(*this, message);
}

END_CS_NAMESPACE