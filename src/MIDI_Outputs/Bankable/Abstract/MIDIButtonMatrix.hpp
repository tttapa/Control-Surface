#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/ButtonMatrix.hpp>
#include <Helpers/Array.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     ButtonMatrix
 */
template <class BankAddress, class Sender, uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public BankAddress,
                         public MIDIOutputElement,
                         public ButtonMatrix<nb_rows, nb_cols> {

  protected:
    /**
     * @brief   Create a new Bankable MIDIButtonMatrix.
     * 
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW as outputs (Lo-Z).
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
    MIDIButtonMatrix(const BankAddress &bankAddress,
                     const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins, const Sender &sender)
        : BankAddress{bankAddress},
          ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins), sender{sender} {}

  public:
    void begin() final override { ButtonMatrix<nb_rows, nb_cols>::begin(); }

    void update() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        if (state == LOW) {
            if (!activeButtons)
                this->lock(); // Don't allow changing of the bank setting
            activeButtons++;
            sender.sendOn(this->getActiveAddress(row, col));
        } else {
            sender.sendOff(this->getActiveAddress(row, col));
            activeButtons--;
            if (!activeButtons)
                this->unlock();
        }
    }

    uint8_t activeButtons = 0;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE