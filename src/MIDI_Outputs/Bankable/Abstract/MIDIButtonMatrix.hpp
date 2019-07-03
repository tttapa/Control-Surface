#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/ButtonMatrix.hpp>
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
class MIDIButtonMatrix : public BankableMIDIOutput,
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
     * @param   channelCN
     *          The MIDI channel [1, 16] and Cable Number [0, 15].
     */
    MIDIButtonMatrix(const OutputBankConfig &config,
                     const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     MIDICNChannel channelCN)
        : BankableMIDIOutput(config), ButtonMatrix<nb_rows, nb_cols>(rowPins,
                                                                     colPins),
          addresses(addresses), baseChannelCN(channelCN) {}

  public:
    void begin() final override { ButtonMatrix<nb_rows, nb_cols>::begin(); }

    void update() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        int8_t address = addresses[row][col];
        MIDICNChannelAddress sendAddress = {address, baseChannelCN};
        if (state == LOW) {
            if (!activeButtons)
                lock(); // Don't allow changing of the bank setting
            sendAddress += getAddressOffset();
            activeButtons++;
            sendOn(sendAddress);
        } else {
            sendAddress += getAddressOffset();
            sendOff(sendAddress);
            activeButtons--;
            if (!activeButtons)
                unlock();
        }
    }

    AddressMatrix<nb_rows, nb_cols> addresses;
    const MIDICNChannel baseChannelCN;
    uint8_t activeButtons = 0;
};

} // namespace Bankable