#pragma once

#include <Banks/BankableMIDIOutputAddressable.hpp>
#include <Def/Def.hpp>
#include <Hardware/ButtonMatrix.h>
#include <Helpers/Array.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

namespace Bankable {

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     ButtonMatrix
 */
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff,
          uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public BankableMIDIOutputAddressable,
                         public MIDIOutputElement,
                         public ButtonMatrix<nb_rows, nb_cols> {

  protected:
    /**
     * @brief   Create a new Bankable MIDIButtonMatrix.
     * 
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix.  
     *          These pins will be used as inputs (Hi-Z), and the
     *          internal pull-up resistor will be enabled.
     * @param   addresses
     *          A 2-dimensional array of the same dimensions as the button
     *          matrix that contains the MIDI address of each button. [0, 127]
     * @param   channel
     *          The MIDI channel. [1, 16]
     */
    MIDIButtonMatrix(const BankConfigAddressable &config,
                     const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     uint8_t channel)
        : BankableMIDIOutputAddressable(config), ButtonMatrix<nb_rows, nb_cols>(
                                                     rowPins, colPins),
          addresses(addresses), baseChannel(channel) {}

  public:
    void begin() final override { ButtonMatrix<nb_rows, nb_cols>::begin(); }

    void update() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        uint8_t address = getAddress(addresses[row][col]);
        if (state == LOW) {
            if (!activeButtons)
                lock(); // Don't allow changing of the bank setting
            activeButtons++;
            sendOn(getChannel(baseChannel), address);
        } else {
            sendOff(getChannel(baseChannel), address);
            activeButtons--;
            if (!activeButtons)
                unlock();
        }
    }

    AddressMatrix<nb_rows, nb_cols> addresses;
    const uint8_t baseChannel;
    uint8_t activeButtons = 0;
};

} // namespace Bankable