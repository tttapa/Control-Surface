#include <MIDI_Outputs/CCButton.hpp>
#include <MIDI_Outputs/CCButtonLatched.hpp>
#include <MIDI_Outputs/CCButtonLatching.hpp>
#include <MIDI_Outputs/CCButtonMatrix.hpp>
#include <MIDI_Outputs/CCButtons.hpp>
#include <MIDI_Outputs/CCIncrementDecrementButtons.hpp>

#include <MIDI_Outputs/CCPotentiometer.hpp>

#include <MIDI_Outputs/NoteButton.hpp>
#include <MIDI_Outputs/NoteButtonLatched.hpp>
#include <MIDI_Outputs/NoteButtonLatching.hpp>
#include <MIDI_Outputs/NoteButtonMatrix.hpp>
#include <MIDI_Outputs/NoteButtons.hpp>
#include <MIDI_Outputs/NoteChordButton.hpp>

#include <MIDI_Outputs/PBPotentiometer.hpp>

#include <MIDI_Outputs/Bankable/CCButton.hpp>
#include <MIDI_Outputs/Bankable/CCButtonLatched.hpp>
#include <MIDI_Outputs/Bankable/CCButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/CCButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/CCButtons.hpp>

#include <MIDI_Outputs/Bankable/CCIncrementDecrementButtons.hpp>
#include <MIDI_Outputs/Bankable/CCPotentiometer.hpp>
#include <MIDI_Outputs/ManyAddresses/CCPotentiometer.hpp>

#include <MIDI_Outputs/Bankable/NoteButton.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonLatched.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/NoteButtons.hpp>
#include <MIDI_Outputs/Bankable/NoteChordButton.hpp>

#include <MIDI_Outputs/Bankable/PBPotentiometer.hpp>

#include <Encoder.h>
//
#include <MIDI_Outputs/Bankable/CCRotaryEncoder.hpp>
#include <MIDI_Outputs/CCRotaryEncoder.hpp>

#include <gtest-wrapper.h>

using namespace CS;

TEST(Construction, MIDIOutputs) {
    const pin_t pin = 0;
    const MIDICNChannelAddress address = {};
    const RelativeMIDICNChannelAddress relAddress = {};
    const MIDICNChannel cnChannel = {};
    uint8_t u8 = 0;
    const PinList<3> rowPins3 = {1, 2, 3};
    const PinList<4> colPins4 = {4, 5, 6, 7};
    const AddressMatrix<3, 4> addressMatrix34 = {{
        {11, 12, 13, 14},
        {21, 22, 23, 24},
        {31, 32, 33, 34},
    }};
    Bank<4> bank{8};

    // CC ----------------------------------------------------------------------
    CCButton{pin, address};
    CCButtonLatched{pin, address};
    CCButtonLatching{pin, address};
    CCButtonMatrix<3, 4>{rowPins3, colPins4, addressMatrix34, cnChannel};
    CCButtons<3>{{pin, pin, pin}, address, relAddress};
    CCIncrementDecrementButtons{{pin, pin}, address, u8, address};

    CCPotentiometer{pin, address};

    CCRotaryEncoder{EncoderPinList{pin, pin}, address, u8, u8};

    // Note --------------------------------------------------------------------
    NoteButton{pin, address};
    NoteButtonLatched{pin, address};
    NoteButtonLatching{pin, address};
    NoteButtonMatrix<3, 4>{rowPins3, colPins4, addressMatrix34, cnChannel};
    NoteButtons<3>{{pin, pin, pin}, address, relAddress};

    // PB ----------------------------------------------------------------------
    PBPotentiometer{pin, cnChannel};

    // Bankable::CC ------------------------------------------------------------
    Bankable::CCButton{bank, pin, address};
    Bankable::CCButtonLatched{bank, pin, address};
    Bankable::CCButtonLatching{bank, pin, address};
    Bankable::CCButtonMatrix<3, 4>{bank, rowPins3, colPins4, addressMatrix34,
                                   cnChannel};
    Bankable::CCButtons<3>{bank, {pin, pin, pin}, address, relAddress};
    Bankable::CCIncrementDecrementButtons{
        bank, {pin, pin}, address, u8, address};

    Bankable::CCPotentiometer{bank, pin, address};

    Bankable::CCRotaryEncoder{bank, EncoderPinList{pin, pin}, address, u8, u8};

    // Bankable::Note ----------------------------------------------------------
    Bankable::NoteButton{bank, pin, address};
    Bankable::NoteButtonLatched{bank, pin, address};
    Bankable::NoteButtonLatching{bank, pin, address};
    Bankable::NoteButtonMatrix<3, 4>{bank, rowPins3, colPins4, addressMatrix34,
                                     cnChannel};
    Bankable::NoteButtons<3>{bank, {pin, pin, pin}, address, relAddress};

    // Bankable::PB ------------------------------------------------------------
    Bankable::PBPotentiometer{bank, pin, cnChannel};
}