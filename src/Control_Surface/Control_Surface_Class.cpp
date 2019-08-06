#include "Control_Surface_Class.hpp"
#include <Hardware/ExtendedInputOutput/ExtendedIOElement.hpp>
#include <MIDI_Constants/Control_Change.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>
#include <MIDI_Inputs/MIDIInputElementPC.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Selectors/Selector.hpp>

#include <Arduino.h>

Control_Surface_ &Control_Surface_::getInstance() {
    static Control_Surface_ instance;
    return instance;
}

void Control_Surface_::begin() {
#if defined(ARDUINO) && defined(DEBUG_OUT)
    DEBUG_OUT.begin(defaultBaudRate);
    delay(250);
#endif
#ifdef ARDUINO_ARCH_ESP32
    analogReadResolution(10);
#endif
    ExtendedIOElement::beginAll();
    MIDI().begin(); // initialize the MIDI interface
    MIDI().setCallbacks(this);
    DisplayInterface::beginAll(); // initialize all displays
    MIDIInputElementCC::beginAll();
    MIDIInputElementPC::beginAll();
    MIDIInputElementChannelPressure::beginAll();
    MIDIInputElementNote::beginAll();
    Updatable<>::beginAll();
    Updatable<Potentiometer>::beginAll();
    Updatable<MotorFader>::beginAll();
    Updatable<Display>::beginAll();
    potentiometerTimer.begin();
    displayTimer.begin();
}

void Control_Surface_::loop() {
    Updatable<>::updateAll();
    if (potentiometerTimer)
        Updatable<Potentiometer>::updateAll();
    updateMidiInput();
    updateInputs();
    if (displayTimer)
        updateDisplays();
}

MIDI_Interface &Control_Surface_::MIDI() {
    MIDI_Interface *midi = MIDI_Interface::getDefault();
    if (midi == nullptr)
        FATAL_ERROR(F("No default MIDI interface is selected."), 0xDEAD);

    return *midi;
}

void Control_Surface_::updateMidiInput() {
    MIDI_Interface &midi = MIDI();
    midi.update();
}

void Control_Surface_::onChannelMessage(MIDI_Interface &midi) {
    ChannelMessage midichmsg = midi.getChannelMessage();
    ChannelMessageMatcher midimsg = {midichmsg};

#ifdef DEBUG_MIDI_PACKETS
    // TODO: print CN
    if (midimsg.type != PROGRAM_CHANGE && midimsg.type != CHANNEL_PRESSURE)
        DEBUG(">>> " << hex << +midichmsg.header << ' ' << +midimsg.data1 << ' '
                     << +midimsg.data2 << dec);
    else
        DEBUG(">>> " << hex << +midichmsg.header << ' ' << +midimsg.data1
                     << dec);
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

void Control_Surface_::onSysExMessage(MIDI_Interface &midi) {
    // System Exclusive
    SysExMessage msg = midi.getSysExMessage();
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
    // TODO: handle SysEx input
}

void Control_Surface_::onRealtimeMessage(MIDI_Interface &midi,
                                         uint8_t message) {
    RealTimeMessage rtMessage = {message, midi.getCN()};
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
}

void Control_Surface_::updateDisplays() {
    DisplayInterface *previousDisplay = nullptr;
    for (DisplayElement &displayElement : DisplayElement::getAll()) {
        DisplayInterface *thisDisplay = &displayElement.getDisplay();
        if (thisDisplay != previousDisplay) {
            if (previousDisplay)
                previousDisplay->display();
            thisDisplay->clearAndDrawBackground();
        }
        displayElement.draw();
        previousDisplay = thisDisplay;
    }
    if (previousDisplay)
        previousDisplay->display();
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
