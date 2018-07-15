#include "Control_Surface_Class.h"
#include "../Banks/BankSelector.hpp"
#include "../MIDI_Inputs/MIDI_Input_CC.hpp"
#include "../MIDI_Inputs/MIDI_Input_Note.hpp"
#include "../MIDI_Outputs/MIDI_Control_Element.h"

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
#if defined(PrintStream_h) && defined(ARDUINO)
    DEBUG_OUT.begin(115200);
    DEBUG_OUT << leadingzeros;
#endif
}

void Control_Surface_::loop() {
    if (midi == nullptr)
        begin(); // make sure that midi != nullptr

    updateControls();
    updateSelectors();

    updateMidiInput();
    updateInputs();
    updateDisplays();
}

MIDI_Interface *Control_Surface_::MIDI() { return midi; }

void Control_Surface_::updateControls() {
    for (MIDI_Control_Element *element = MIDI_Control_Element::getFirst();
         element != nullptr; element = element->getNext())
        element->update();
}

void Control_Surface_::updateSelectors() {
    for (Selector *element = Selector::getFirst(); element != nullptr;
         element = element->getNext())
        element->update();
}

void Control_Surface_::updateMidiInput() {
    MIDI_read_t midiReadResult = midi->read();

    while (midiReadResult != NO_MESSAGE) {
        if (midiReadResult == CHANNEL_MESSAGE) {
            MIDI_message_matcher midimsg(midi->getChannelMessage());

#ifdef DEBUG_MIDI_PACKETS
            if (midimsg.type != PROGRAM_CHANGE &&
                midimsg.type != CHANNEL_PRESSURE)
                DEBUG(">>> " << hex << +midi->getChannelMessage().header << ' '
                             << +midimsg.data1 << ' ' << +midimsg.data2);
            else
                DEBUG(">>> " << hex << +midi->getChannelMessage().header << ' '
                             << +midimsg.data1);
#endif

            if (midimsg.type == CC && midimsg.data1 == 0x79) {
                // Reset All Controllers
                DEBUG("Reset All Controllers");
                /*
        for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst();
             element != nullptr; element = element->getNext())
          element->reset();
        for (MIDI_Input_Element_ChannelPressure *element =
                 MIDI_Input_Element_ChannelPressure::getFirst();
             element != nullptr; element = element->getNext())
          element->reset();
        */
                // TODO

            } else if (midimsg.type == CC && midimsg.data1 == 0x7B) {
                // All Notes off
                /*
        for (MIDI_Input_Element_Note *element =
                 MIDI_Input_Element_Note::getFirst();
             element != nullptr; element = element->getNext())
          element->reset();
        */
                // TODO

            } else {

                if (midimsg.type == CC) {
                    // Control Change
                    DEBUGFN(F("Updating CC elements with new MIDI message."));
                    MIDI_Input_CC::updateAllWith(midimsg);

                } else if (midimsg.type == NOTE_OFF ||
                           midimsg.type == NOTE_ON) {
                    // Note
                    DEBUGFN(F("Updating Note elements with new MIDI message."));
                    MIDI_Input_Note::updateAllWith(midimsg);

                } else if (midimsg.type == CHANNEL_PRESSURE) {
                    // Channel Pressure
                    /*
          for (MIDI_Input_Element_ChannelPressure *element =
                   MIDI_Input_Element_ChannelPressure::getFirst();
               element != nullptr; element = element->getNext()) {
            if (element->updateWith(midimsg))
              break;
          }
          */
                    // TODO
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
    MIDI_Input_CC::updateAll();
    MIDI_Input_Note::updateAll();
    /*
    for (MIDI_Input_Element_ChannelPressure *element =
             MIDI_Input_Element_ChannelPressure::getFirst();
         element != nullptr; element = element->getNext())
        element->update();
    */
    // TODO
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
