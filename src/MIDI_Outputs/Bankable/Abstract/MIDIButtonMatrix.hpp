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
 * @todo    Documentation
 * @see     ButtonMatrix
 * 
 * @tparam  BankAddress
 *          The bankable address object containing the addresses of all buttons,
 *          as well as a reference to the bank this element belongs to.
 * @tparam  Sender
 *          The MIDI Sender class.
 * @tparam  nb_rows 
 *          The number of rows of the button matrix.
 * @tparam  nb_cols
 *          The number of columns of the button matrix.
 */
template <class BankAddress, class Sender, uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public MIDIOutputElement,
                         public ButtonMatrix<nb_rows, nb_cols> {

  protected:
    /**
     * @brief   Create a new Bankable MIDIButtonMatrix.
     * 
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW as outputs (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix.  
     *          These pins will be used as inputs (Hi-Z), and the
     *          internal pull-up resistor will be enabled.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButtonMatrix(const BankAddress &bankAddress,
                     const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins, const Sender &sender)
        : ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins),
          address{bankAddress}, sender{sender} {}

  public:
    void begin() override { ButtonMatrix<nb_rows, nb_cols>::begin(); }

    void update() override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        if (state == LOW) {
            if (!activeButtons)
                address.lock(); // Don't allow changing of the bank setting
            activeButtons++;
            sender.sendOn(address.getActiveAddress(row, col));
        } else {
            sender.sendOff(address.getActiveAddress(row, col));
            activeButtons--;
            if (!activeButtons)
                address.unlock();
        }
    }

    BankAddress address;
    uint16_t activeButtons = 0;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE