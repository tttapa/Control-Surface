#include "Control_Surface_Class.h"
#include <Hardware/ExtendedInputOutput/ExtendedIOElement.h>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Selectors/Selector.hpp>

#include <Arduino.h>

Control_Surface_ &Control_Surface_::getInstance() {
    static Control_Surface_ instance;
    return instance;
}

void Control_Surface_::begin() {
#if defined(PrintStream_h) && defined(ARDUINO) && defined(DEBUG_OUT)
    DEBUG_OUT.begin(115200);
#endif
#ifdef ARDUINO_ARCH_ESP32
    analogReadResolution(10);
#endif
    ExtendedIOElement::beginAll();
    MIDI().begin(); // initialize the MIDI interface
    MIDI().setCallbacks(this);
    DisplayInterface::beginAll(); // initialize all displays
    // MIDIOutputElement::beginAll();
    MIDIInputElementCC::beginAll();
    MIDIInputElementChannelPressure::beginAll();
    MIDIInputElementNote::beginAll();
    // Selector::beginAll();
    Updatable::beginAll();
}

void Control_Surface_::loop() {
    // MIDIOutputElement::updateAll(); // TODO
    // Selector::updateAll();
    Updatable::updateAll();
    updateMidiInput();
    updateInputs();
    updateDisplays();
}

MIDI_Interface &Control_Surface_::MIDI() {
    MIDI_Interface *midi = MIDI_Interface::getDefault();
    if (midi == nullptr) {
        DEBUGFN("Error: no default MIDI interface is selected.");
        FATAL_ERROR();
    }
    return *midi;
}

void Control_Surface_::updateMidiInput() {
    MIDI_Interface &midi = MIDI();
    midi.update();
}

void Control_Surface_::onChannelMessage(MIDI_Interface &midi) {
    MIDI_message midichmsg = midi.getChannelMessage();
    MIDI_message_matcher midimsg(midichmsg);

#ifdef DEBUG_MIDI_PACKETS
    if (midimsg.type != PROGRAM_CHANGE && midimsg.type != CHANNEL_PRESSURE)
        DEBUG(">>> " << hex << +midichmsg.header << ' ' << +midimsg.data1 << ' '
                     << +midimsg.data2 << dec);
    else
        DEBUG(">>> " << hex << +midichmsg.header << ' ' << +midimsg.data1
                     << dec);
#endif

    if (midimsg.type == CC && midimsg.data1 == 0x79) {
        // Reset All Controllers
        DEBUG("Reset All Controllers");
        MIDIInputElementCC::resetAll();
        MIDIInputElementChannelPressure::resetAll();

    } else if (midimsg.type == CC && midimsg.data1 == 0x7B) {
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
        }
    }
}

void Control_Surface_::onSysExMessage(MIDI_Interface &midi) {

    // System Exclusive
#ifdef DEBUG_MIDI_PACKETS
    const uint8_t *data = midi.getSysExBuffer();
    size_t len = midi.getSysExLength();
    DEBUG_OUT << hex;
    for (size_t i = 0; i < len; i++)
        DEBUG_OUT << data[i] << ' ';
    DEBUG_OUT << dec << endl;
#endif
}

void Control_Surface_::updateInputs() {
    MIDIInputElementCC::updateAll();
    MIDIInputElementNote::updateAll();
    MIDIInputElementChannelPressure::updateAll();
}

void Control_Surface_::updateDisplays() {
    static unsigned long previousRefresh = millis();

    if (millis() - previousRefresh < 1000 / MAX_FPS)
        return;
    previousRefresh += 1000 / MAX_FPS;

    DisplayInterface::clearAll();
    DisplayInterface::drawAllBackgrounds();
    DisplayElement::drawAll();
    DisplayInterface::displayAll();
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
