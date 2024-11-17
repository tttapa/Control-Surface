#include <Control_Surface.h>

void setup() {
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
    Bank<4> bank{8};

    // CC ----------------------------------------------------------------------
    CCButton{pin, address};
    CCButtonLatched{pin, address};
    CCButtonLatching{pin, address};
    CCButtonMatrix<3, 4>{rowPins3, colPins4, addressMatrix34, cnChannel};
    CCButtons<3>{{pin, pin, pin}, address, relAddress};
    CCIncrementDecrementButtons{{pin, pin}, address, u8, address};

    CCPotentiometer{pin, address};

    CCRotaryEncoder{EncoderPinList{pin, pin}, address, i8, u8};

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
    Bankable::CCButtonLatched<4>{bank, pin, address};
    Bankable::CCButtonLatching{bank, pin, address};
    Bankable::CCButtonMatrix<3, 4>{bank, rowPins3, colPins4, addressMatrix34,
                                   cnChannel};
    Bankable::CCButtons<3>{bank, {pin, pin, pin}, address, relAddress};
    Bankable::CCIncrementDecrementButtons{
        bank, {pin, pin}, address, u8, address};

    Bankable::CCPotentiometer{bank, pin, address};

    Bankable::CCRotaryEncoder{bank, EncoderPinList{pin, pin}, address, i8, u8};

    // Bankable::Note ----------------------------------------------------------
    Bankable::NoteButton{bank, pin, address};
    Bankable::NoteButtonLatched<4>{bank, pin, address};
    Bankable::NoteButtonLatching{bank, pin, address};
    Bankable::NoteButtonMatrix<3, 4>{bank, rowPins3, colPins4, addressMatrix34,
                                     cnChannel};
    Bankable::NoteButtons<3>{bank, {pin, pin, pin}, address, relAddress};

    // Bankable::PB ------------------------------------------------------------
    Bankable::PBPotentiometer{bank, pin, cnChannel};

    // ManyAddresses::CC -------------------------------------------------------
    Bankable::ManyAddresses::CCButton<4>{bank, pin, addresses};

    Bankable::ManyAddresses::CCPotentiometer<4>{bank, pin, addresses};
};
void loop(){};