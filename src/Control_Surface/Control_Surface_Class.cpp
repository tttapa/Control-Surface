#include "Control_Surface_Class.hpp"
#include <AH/Debug/Debug.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedIOElement.hpp>
#include <AH/Hardware/FilteredAnalog.hpp>
#include <MIDI_Constants/Control_Change.hpp>
#include <MIDI_Inputs/MIDIInputElement.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Selectors/Selector.hpp>

#include <AH/Arduino-Wrapper.h>

BEGIN_CS_NAMESPACE

using AH::ExtendedIOElement;

Control_Surface_ &Control_Surface_::getInstance() {
    static Control_Surface_ instance;
    return instance;
}

void Control_Surface_::begin() {
#if defined(ARDUINO) && defined(DEBUG_OUT)
    DEBUG_OUT.begin(AH::defaultBaudRate);
    delay(250);
#endif

    connectDefaultMIDI_Interface();

    FilteredAnalog<>::setupADC();
    ExtendedIOElement::beginAll();
    Updatable<MIDI_Interface>::beginAll();
    DisplayInterface::beginAll(); // initialize all displays
    MIDIInputElementNote::beginAll();
    MIDIInputElementKP::beginAll();
    MIDIInputElementCC::beginAll();
    MIDIInputElementPC::beginAll();
    MIDIInputElementCP::beginAll();
    MIDIInputElementPB::beginAll();
    MIDIInputElementSysEx::beginAll();
    Updatable<>::beginAll();
    Updatable<Potentiometer>::beginAll();
    Updatable<MotorFader>::beginAll();
    Updatable<Display>::beginAll();
    potentiometerTimer.begin();
    displayTimer.begin();
}

bool Control_Surface_::connectDefaultMIDI_Interface() {
    if (hasSinkPipe() || hasSourcePipe())
        return false;
    auto def = MIDI_Interface::getDefault();
    if (def == nullptr) {
        FATAL_ERROR(F("No default MIDI Interface"), 0xF123);
        return false;
    }
    *this << inpipe << *def;
    *this >> outpipe >> *def;
    return true;
}

void Control_Surface_::disconnectMIDI_Interfaces() {
    disconnectSinkPipes();
    disconnectSourcePipes();
}

void Control_Surface_::loop() {
    ExtendedIOElement::updateAllBufferedInputs();
    Updatable<>::updateAll();
    if (potentiometerTimer)
        Updatable<Potentiometer>::updateAll();
    updateMidiInput();
    updateInputs();
    if (displayTimer)
        updateDisplays();
    ExtendedIOElement::updateAllBufferedOutputs();
}

void Control_Surface_::updateMidiInput() {
    Updatable<MIDI_Interface>::updateAll();
}

void Control_Surface_::sendImpl(uint8_t header, uint8_t d1, uint8_t d2,
                                uint8_t cn) {
    this->sourceMIDItoPipe(ChannelMessage{header, d1, d2, cn});
}
void Control_Surface_::sendImpl(uint8_t header, uint8_t d1, uint8_t cn) {
    this->sourceMIDItoPipe(ChannelMessage{header, d1, 0x00, cn});
}
void Control_Surface_::sendImpl(const uint8_t *data, size_t length,
                                uint8_t cn) {
    this->sourceMIDItoPipe(SysExMessage{data, length, cn});
}
void Control_Surface_::sendImpl(uint8_t rt, uint8_t cn) {
    this->sourceMIDItoPipe(RealTimeMessage{rt, cn});
}

void Control_Surface_::sinkMIDIfromPipe(ChannelMessage midichmsg) {
    ChannelMessageMatcher midimsg = {midichmsg};

#ifdef DEBUG_MIDI_PACKETS
    // TODO: print CN
    if (midimsg.type != PROGRAM_CHANGE && midimsg.type != CHANNEL_PRESSURE)
        DEBUG(">>> " << hex << midichmsg.header << ' ' << midimsg.data1 << ' '
                     << midimsg.data2 << dec);
    else
        DEBUG(">>> " << hex << midichmsg.header << ' ' << midimsg.data1 << dec);
#endif

    // If the Channel Message callback exists, call it to see if we have to
    // continue handling it.
    if (channelMessageCallback && channelMessageCallback(midichmsg))
        return;

    if (midimsg.type == MIDIMessageType::CONTROL_CHANGE &&
        midimsg.data1 == MIDI_CC::Reset_All_Controllers) {
        // Reset All Controllers
        DEBUG(F("Reset All Controllers"));
        MIDIInputElementCC::resetAll();
        MIDIInputElementCP::resetAll();
    } else if (midimsg.type == MIDIMessageType::CONTROL_CHANGE &&
               midimsg.data1 == MIDI_CC::All_Notes_Off) {
        // All Notes Off
        MIDIInputElementNote::resetAll();
    } else {
        if (midimsg.type == MIDIMessageType::CONTROL_CHANGE) {
            // Control Change
            DEBUGFN(F("Updating CC elements with new MIDI message."));
            MIDIInputElementCC::updateAllWith(midimsg);

        } else if (midimsg.type == MIDIMessageType::NOTE_OFF ||
                   midimsg.type == MIDIMessageType::NOTE_ON) {
            // Note
            DEBUGFN(F("Updating Note elements with new MIDI message."));
            MIDIInputElementNote::updateAllWith(midimsg);

        } else if (midimsg.type == MIDIMessageType::CHANNEL_PRESSURE) {
            // Channel Pressure
            DEBUGFN(F("Updating Channel Pressure elements with new "
                      "MIDI message."));
            MIDIInputElementCP::updateAllWith(midimsg);
        } else if (midimsg.type == MIDIMessageType::PROGRAM_CHANGE) {
            // Channel Pressure
            DEBUGFN(F("Updating Program Change elements with new "
                      "MIDI message."));
            MIDIInputElementPC::updateAllWith(midimsg);
        }
    }
}

void Control_Surface_::sinkMIDIfromPipe(SysExMessage msg) {
    // System Exclusive
#ifdef DEBUG_MIDI_PACKETS
    const uint8_t *data = msg.data;
    size_t len = msg.length;
    // TODO: print CN
    DEBUG_OUT << hex;
    for (size_t i = 0; i < len; i++)
        DEBUG_OUT << data[i] << ' ';
    DEBUG_OUT << dec << endl;
#endif
    // If the SysEx Message callback exists, call it to see if we have to
    // continue handling it.
    if (sysExMessageCallback && sysExMessageCallback(msg))
        return;
    MIDIInputElementSysEx::updateAllWith(msg);
}

void Control_Surface_::sinkMIDIfromPipe(RealTimeMessage rtMessage) {
    // If the Real-Time Message callback exists, call it to see if we have to
    // continue handling it.
    if (realTimeMessageCallback && realTimeMessageCallback(rtMessage))
        return;
    // TODO: handle Real-Time input
}

void Control_Surface_::updateInputs() {
    MIDIInputElementNote::updateAll();
    MIDIInputElementKP::updateAll();
    MIDIInputElementCC::updateAll();
    MIDIInputElementPC::updateAll();
    MIDIInputElementCP::updateAll();
    MIDIInputElementPB::updateAll();
    MIDIInputElementSysEx::updateAll();
}

void Control_Surface_::updateDisplays() {
    DisplayInterface *previousDisplay = nullptr;
    for (DisplayElement &displayElement : DisplayElement::getAll()) {
        DisplayInterface *thisDisplay = &displayElement.getDisplay();
        if (!thisDisplay->isEnabled())
            continue;
        if (thisDisplay != previousDisplay) {
            if (previousDisplay)
                previousDisplay->display();
            previousDisplay = thisDisplay;
            thisDisplay->clearAndDrawBackground();
        }
        displayElement.draw();
    }
    if (previousDisplay)
        previousDisplay->display();
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();

END_CS_NAMESPACE