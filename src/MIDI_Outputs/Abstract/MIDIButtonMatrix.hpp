#pragma once

#include <AH/Hardware/ButtonMatrix.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     AH::ButtonMatrix
 */
template <class Sender, uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public MIDIOutputElement,
                         public AH::ButtonMatrix<nb_rows, nb_cols> {

  protected:
    /**
     * @brief   Construct a new MIDIButtonMatrix.
     * 
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW as outputs (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix.  
     *          These pins will be used as inputs (Hi-Z), and the internal 
     *          pull-up resistor will be enabled.
     * @param   addresses
     *          A matrix containing the address corresponding to each button.
     * @param   channelCN
     *          The MIDI channel and optional cable number for all buttons.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButtonMatrix(const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     MIDIChannelCable channelCN, const Sender &sender)
        : AH::ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins),
          addresses(addresses), baseChannelCN(channelCN), sender(sender) {}

  public:
    void begin() final override { AH::ButtonMatrix<nb_rows, nb_cols>::begin(); }

    void update() final override {
        AH::ButtonMatrix<nb_rows, nb_cols>::update();
    }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        int8_t address = addresses[row][col];
        if (state == LOW)
            sender.sendOn({address, baseChannelCN});
        else
            sender.sendOff({address, baseChannelCN});
    }

    AddressMatrix<nb_rows, nb_cols> addresses;
    MIDIChannelCable baseChannelCN;

  public:
    Sender sender;
};

END_CS_NAMESPACE