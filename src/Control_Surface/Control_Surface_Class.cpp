#include "Control_Surface_Class.h"
#include <Banks/BankSelector.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

Control_Surface_ &Control_Surface_::getInstance() {
    static Control_Surface_ instance;
    return instance;
}

Control_Surface_::~Control_Surface_() { delete new_midi; }

void Control_Surface_::begin() {
    if (midi == nullptr) { // if this is the first time that `begin` is executed
        midi = MIDI_Interface::getDefault();  // use the default MIDI interface
        if (midi == nullptr) {                // if it doesn't exist
            new_midi = new USBMIDI_Interface; // create a new USB MIDI interface
            midi = new_midi;
        }
        midi->begin(); // initialize the MIDI interface
    }
#if defined(PrintStream_h) && defined(ARDUINO) && defined(DEBUG_OUT)
    DEBUG_OUT.begin(115200);
    DEBUG_OUT << leadingzeros;
#endif
}

void Control_Surface_::loop() {
    updateControls();
    updateSelectors();

    updateMidiInput();
    updateInputs();
    updateDisplays();
}

MIDI_Interface &Control_Surface_::MIDI() {
    if (midi == nullptr)
        begin(); // make sure that midi != nullptr
    return *midi;
}

void Control_Surface_::setMIDI_Interface(MIDI_Interface &midi) {
    delete new_midi;
    new_midi = nullptr;
    this->midi = &midi;
}

void Control_Surface_::updateControls() {
    MIDIOutputElement::updateAll();
}

void Control_Surface_::updateSelectors() {
    for (Selector *element = Selector::getFirst(); element != nullptr;
         element = element->getNext())
        element->update();
}

void Control_Surface_::updateMidiInput() {
    MIDI_read_t midiReadResult = MIDI().read();

    while (midiReadResult != NO_MESSAGE) {
        if (midiReadResult == CHANNEL_MESSAGE) {
            MIDI_message midichmsg = midi->getChannelMessage();
            MIDI_message_matcher midimsg(midichmsg);

#ifdef DEBUG_MIDI_PACKETS
            if (midimsg.type != PROGRAM_CHANGE &&
                midimsg.type != CHANNEL_PRESSURE)
                DEBUG(">>> " << hex << +midichmsg.header << ' '
                             << +midimsg.data1 << ' ' << +midimsg.data2);
            else
                DEBUG(">>> " << hex << +midichmsg.header << ' '
                             << +midimsg.data1);
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

                } else if (midimsg.type == NOTE_OFF ||
                           midimsg.type == NOTE_ON) {
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

        } else if (midiReadResult == SYSEX_MESSAGE) {
            // System Exclusive
#ifdef DEBUG_MIDI_PACKETS
            const uint8_t *data = midi->getSysExBuffer();
            size_t len = midi->getSysExLength();
            DEBUG_OUT << hex;
            for (size_t i = 0; i < len; i++)
                DEBUG_OUT << data[i] << ' ';
            DEBUG_OUT << dec << endl;
#endif
        }
        midiReadResult = midi->read();
    }
}

void Control_Surface_::updateInputs() {
    MIDIInputElementCC::updateAll();
    MIDIInputElementNote::updateAll();
    /*
    for (MIDIInputElement_ChannelPressure *element =
             MIDIInputElement_ChannelPressure::getFirst();
         element != nullptr; element = element->getNext())
        element->update();
    */
    // TODO
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
