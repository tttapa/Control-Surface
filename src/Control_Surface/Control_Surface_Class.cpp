#include "Control_Surface_Class.hpp"
#include <AH/Debug/Debug.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedIOElement.hpp>
#include <AH/Hardware/FilteredAnalog.hpp>
#include <MIDI_Constants/Control_Change.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>
#include <MIDI_Inputs/MIDIInputElementPC.hpp>
#include <MIDI_Inputs/MIDIInputElementSysEx.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Selectors/Selector.hpp>

#include <Arduino.h>

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
    // Initialize the hardware (ADC and Extended IO)
    FilteredAnalog<>::setupADC();
    ExtendedIOElement::beginAll();
    // Initialize the MIDI interfaces
    Updatable<MIDI_Interface>::beginAll();
    connectDefaultMIDI();
    // Initialize the displays
    DisplayInterface::beginAll();
    // Initialize the MIDI Input Elements
    MIDIInputElementCC::beginAll();
    MIDIInputElementPC::beginAll();
    MIDIInputElementChannelPressure::beginAll();
    MIDIInputElementNote::beginAll();
    MIDIInputElementSysEx::beginAll();
    // Initialize the Updatables (including MIDI Output Elements)
    Updatable<>::beginAll();
    Updatable<Potentiometer>::beginAll();
    Updatable<MotorFader>::beginAll();
    Updatable<Display>::beginAll(); // Display elements
    // Reset the timers
    potentiometerTimer.begin();
    displayTimer.begin();
}

bool Control_Surface_::connectDefaultMIDI() {
    if (MIDI_Interface::getDefault()) {
        auto &midi_interface = *MIDI_Interface::getDefault();
        auto &control_surface = *this;
        pipe.first.disconnect();
        pipe.second.disconnect();
        // Connect Control Surface to the default MIDI interface:
        control_surface | pipe | midi_interface;
        return true;
    } else {
        DEBUG(F("Warning: no default MIDI interface, Control Surface not "
                "connected"));
        return false;
    }
}

void Control_Surface_::loop() {
    // Update MIDI Output Elements
    Updatable<>::updateAll();
    if (potentiometerTimer)
        Updatable<Potentiometer>::updateAll();
    // Read MIDI input
    Updatable<MIDI_Interface>::updateAll();
    // Update MIDI Input Elements
    updateInputs();
    // Update the displays
    if (displayTimer)
        updateDisplays();
}

MIDI_Sender &Control_Surface_::MIDI() { return *this; }

void Control_Surface_::sinkMIDIfromPipe(ChannelMessage midichmsg) {
    ChannelMessageMatcher midimsg = midichmsg;

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

    if (midimsg.type == CC && midimsg.data1 == 0x79) {
        // Reset All Controllers
        DEBUG(F("Reset All Controllers"));
        MIDIInputElementCC::resetAll();
        MIDIInputElementChannelPressure::resetAll();
    } else if (midimsg.type == CC && midimsg.data1 == MIDI_CC::All_Notes_Off) {
        MIDIInputElementNote::resetAll();
    } else {
        if (midimsg.type == CC) {
            // Control Change
            DEBUGFN(F("Updating CC elements with new MIDI message."));
            MIDIInputElementCC::updateAllWith(midimsg);

        } else if (midimsg.type == NOTE_OFF || midimsg.type == NOTE_ON) {
            // Note
            DEBUGFN(F("Updating Note elements with new MIDI message."));
            MIDIInputElementNote::updateAllWith(midimsg);

        } else if (midimsg.type == CHANNEL_PRESSURE) {
            // Channel Pressure
            DEBUGFN(F("Updating Channel Pressure elements with new "
                      "MIDI message."));
            MIDIInputElementChannelPressure::updateAllWith(midimsg);
        } else if (midimsg.type == PROGRAM_CHANGE) {
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
    MIDIInputElementCC::updateAll();
    MIDIInputElementNote::updateAll();
    MIDIInputElementChannelPressure::updateAll();
    MIDIInputElementPC::updateAll();
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