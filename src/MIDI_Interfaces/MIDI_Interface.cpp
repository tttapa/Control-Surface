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

MIDI_Interface *MIDI_Interface::getDefault() {
#ifndef ARDUINO
    return DefaultMIDI_Interface == nullptr
               ? dynamic_cast<MIDI_Interface *>(updatables.getLast())
               : DefaultMIDI_Interface;
#else
    return DefaultMIDI_Interface == nullptr
               ? static_cast<MIDI_Interface *>(updatables.getLast())
               : DefaultMIDI_Interface;
#endif
}

MIDI_Interface *MIDI_Interface::DefaultMIDI_Interface = nullptr;

// -------------------------------------------------------------------------- //

// Handling incoming MIDI events

bool MIDI_Interface::onChannelMessage(ChannelMessage message) {
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onChannelMessage(*this, message);
    return true;
}

bool MIDI_Interface::onSysExMessage(SysExMessage message) {
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onSysExMessage(*this, message);
    return true;
}

bool MIDI_Interface::onRealTimeMessage(RealTimeMessage message) {
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onRealTimeMessage(*this, message);
    return true;
}

END_CS_NAMESPACE