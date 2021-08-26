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
#include <MIDI_Outputs/PCButton.hpp>

#include <MIDI_Outputs/ProgramChanger.hpp>

#include <MIDI_Outputs/Bankable/CCButton.hpp>
#include <MIDI_Outputs/Bankable/CCButtonLatched.hpp>
#include <MIDI_Outputs/Bankable/CCButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/CCButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/CCButtons.hpp>

#include <MIDI_Outputs/Bankable/CCIncrementDecrementButtons.hpp>
#include <MIDI_Outputs/Bankable/CCPotentiometer.hpp>
#include <MIDI_Outputs/ManyAddresses/CCButton.hpp>
#include <MIDI_Outputs/ManyAddresses/CCButtonMatrix.hpp>
#include <MIDI_Outputs/ManyAddresses/CCIncrementDecrementButtons.hpp>
#include <MIDI_Outputs/ManyAddresses/CCPotentiometer.hpp>

#include <MIDI_Outputs/Bankable/NoteButton.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonLatched.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/NoteButtons.hpp>
#include <MIDI_Outputs/Bankable/NoteChordButton.hpp>
#include <MIDI_Outputs/ManyAddresses/NoteButton.hpp>
#include <MIDI_Outputs/ManyAddresses/NoteButtonMatrix.hpp>

#include <MIDI_Outputs/Bankable/PBPotentiometer.hpp>
#include <MIDI_Outputs/Bankable/PCButton.hpp>
#include <MIDI_Outputs/ManyAddresses/PBPotentiometer.hpp>
#include <MIDI_Outputs/ManyAddresses/PCButton.hpp>

#include <MIDI_Outputs/Bankable/CCAbsoluteEncoder.hpp>
#include <MIDI_Outputs/Bankable/CCRotaryEncoder.hpp>
#include <MIDI_Outputs/CCAbsoluteEncoder.hpp>
#include <MIDI_Outputs/CCRotaryEncoder.hpp>
#include <MIDI_Outputs/ManyAddresses/CCAbsoluteEncoder.hpp>
#include <MIDI_Outputs/ManyAddresses/CCRotaryEncoder.hpp>

#include <MIDI_Outputs/Bankable/PBAbsoluteEncoder.hpp>
#include <MIDI_Outputs/ManyAddresses/PBAbsoluteEncoder.hpp>
#include <MIDI_Outputs/PBAbsoluteEncoder.hpp>

#include <MIDI_Outputs/Bankable/ProgramChanger.hpp>

#include <Selectors/EncoderSelector.hpp>
#include <Selectors/IncrementDecrementSelector.hpp>
#include <Selectors/IncrementSelector.hpp>
#include <Selectors/ManyButtonsSelector.hpp>
#include <Selectors/ProgramChangeSelector.hpp>
#include <Selectors/SwitchSelector.hpp>

#include <Selectors/LEDs/SelectorLEDs.hpp>

#include <gtest/gtest.h>

using namespace CS;

TEST(Construction, MIDIOutputs) {
    const pin_t pin = 0;
    const MIDIAddress address = {};
    const RelativeMIDIAddress relAddress = {};
    const MIDIChannelCable cnChannel = {};
    uint8_t u8 = 0;
    int8_t i8 = 0;
    const PinList<3> rowPins3 = {1, 2, 3};
    const PinList<4> colPins4 = {4, 5, 6, 7};
    const AddressMatrix<3, 4> addressMatrix34 = {{
        {11, 12, 13, 14},
        {21, 22, 23, 24},
        {31, 32, 33, 34},
    }};
    const Array<MIDIAddress, 4> addresses = {{
        1,
        2,
        3,
        4,
    }};
    const Array<uint8_t, 4> u8addresses = {{
        1,
        2,
        3,
        4,
    }};
    const Array<MIDIChannelCable, 4> cnChannels = {{
        {CHANNEL_1, CABLE_1},
        {CHANNEL_2, CABLE_1},
        {CHANNEL_3, CABLE_1},
        {CHANNEL_4, CABLE_1},
    }};
    Bank<4> bank {8};

    // CC ----------------------------------------------------------------------
    CCButton {pin, address};
    CCButtonLatched {pin, address};
    CCButtonLatching {pin, address};
    CCButtonMatrix<3, 4> {rowPins3, colPins4, addressMatrix34, cnChannel};
    CCButtons<3> {{pin, pin, pin}, address, relAddress};
    CCIncrementDecrementButtons {{pin, pin}, address, u8, address};

    CCPotentiometer {pin, address};

    CCRotaryEncoder {{pin, pin}, address, i8, u8};

    // Note --------------------------------------------------------------------
    NoteButton {pin, address};
    NoteButtonLatched {pin, address};
    NoteButtonLatching {pin, address};
    NoteButtonMatrix<3, 4> {rowPins3, colPins4, addressMatrix34, cnChannel};
    NoteButtons<3> {{pin, pin, pin}, address, relAddress};

    // PB ----------------------------------------------------------------------
    PBPotentiometer {pin, cnChannel};

    // PC ----------------------------------------------------------------------
    PCButton {pin, address};
    ProgramChanger<4> {u8addresses, cnChannel};

    // Bankable::CC ------------------------------------------------------------
    Bankable::CCButton {bank, pin, address};
    Bankable::CCButtonLatched<4> {bank, pin, address};
    Bankable::CCButtonLatching {bank, pin, address};
    Bankable::CCButtonMatrix<3, 4> {bank, rowPins3, colPins4, addressMatrix34,
                                    cnChannel};
    Bankable::CCButtons<3> {bank, {pin, pin, pin}, address, relAddress};
    Bankable::CCIncrementDecrementButtons {
        bank, {pin, pin}, address, u8, address};

    Bankable::CCPotentiometer {bank, pin, address};

    Bankable::CCRotaryEncoder {bank, {pin, pin}, address, i8, u8};
    Bankable::CCAbsoluteEncoder<4> {bank, {pin, pin}, address, i8, u8};

    // Bankable::Note ----------------------------------------------------------
    Bankable::NoteButton {bank, pin, address};
    Bankable::NoteButtonLatched<4> {bank, pin, address};
    Bankable::NoteButtonLatching {bank, pin, address};
    Bankable::NoteButtonMatrix<3, 4> {bank, rowPins3, colPins4, addressMatrix34,
                                      cnChannel};
    Bankable::NoteButtons<3> {bank, {pin, pin, pin}, address, relAddress};

    // Bankable::PB ------------------------------------------------------------
    Bankable::PBPotentiometer {bank, pin, cnChannel};

    Bankable::PBAbsoluteEncoder<4> {bank, {pin, pin}, cnChannel, i8, u8};

    // Bankable::PC ------------------------------------------------------------
    Bankable::PCButton {bank, pin, address};

    // ManyAddresses::Note -------------------------------------------------------
    Bankable::ManyAddresses::NoteButton<4> {bank, pin, addresses};
    Bankable::ManyAddresses::NoteButtonMatrix<4, 3, 4> {
        bank,
        rowPins3,
        colPins4,
        {addressMatrix34, addressMatrix34, addressMatrix34, addressMatrix34},
        {cnChannel, cnChannel, cnChannel, cnChannel},
    };

    // ManyAddresses::CC -------------------------------------------------------
    Bankable::ManyAddresses::CCButton<4> {bank, pin, addresses};
    Bankable::ManyAddresses::CCButtonMatrix<4, 3, 4> {
        bank,
        rowPins3,
        colPins4,
        {addressMatrix34, addressMatrix34, addressMatrix34, addressMatrix34},
        {cnChannel, cnChannel, cnChannel, cnChannel},
    };
    Bankable::ManyAddresses::CCIncrementDecrementButtons<4> {
        bank, {pin, pin}, addresses, 4, addresses};

    Bankable::ManyAddresses::CCPotentiometer<4> {bank, pin, addresses};

    Bankable::ManyAddresses::CCRotaryEncoder<4> {
        bank, {pin, pin}, addresses, i8, u8};

    Bankable::ManyAddresses::CCAbsoluteEncoder<4> {
        bank, {pin, pin}, addresses, i8, u8};

    // ManyAddresses::PB -------------------------------------------------------
    Bankable::ManyAddresses::PBPotentiometer<4> {
        bank, pin, {cnChannel, cnChannel, cnChannel, cnChannel}};

    Bankable::ManyAddresses::PBAbsoluteEncoder<4> {
        bank, {pin, pin}, cnChannels, i8, u8};

    // ManyAddresses::PC -------------------------------------------------------
    Bankable::ManyAddresses::PCButton<4> {bank, pin, addresses};
    Bankable::ProgramChanger<4> {bank, u8addresses, cnChannel};
}

TEST(Construction, Selectors) {
    const pin_t pin = 0;
    const MIDIChannelCable cnChannel = {};
    const Wrap wrap = Wrap::NoWrap;
    Bank<4> bank;
    Bank<2> bank2;

    EncoderSelector<4> {bank, {pin, pin, pin}, 4, wrap};
    IncrementDecrementSelector<4> {bank, {pin, pin}, wrap};
    IncrementSelector<4> {bank, pin};
    ManyButtonsSelector<4> {bank, {pin, pin, pin, pin}};
    ProgramChangeSelector<4> {bank, cnChannel};
    SwitchSelector {bank2, pin};

    EncoderSelectorLEDs<4> {
        bank, {pin, pin, pin}, {pin, pin, pin, pin}, 4, wrap};
    IncrementDecrementSelectorLEDs<4> {
        bank, {pin, pin}, {pin, pin, pin, pin}, wrap};
    IncrementSelectorLEDs<4> {bank, pin, {pin, pin, pin, pin}};
    ManyButtonsSelectorLEDs<4> {
        bank, {pin, pin, pin, pin}, {pin, pin, pin, pin}};
    ProgramChangeSelectorLEDs<4> {bank, cnChannel, {pin, pin, pin, pin}};
    SwitchSelectorLED {bank2, pin, pin};
}