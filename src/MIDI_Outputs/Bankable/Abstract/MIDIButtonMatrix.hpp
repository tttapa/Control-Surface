#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/Hardware/ButtonMatrix.hpp>
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation
 * @see     AH::ButtonMatrix
 * 
 * @tparam  BankAddress
 *          The bankable address object containing the addresses of all buttons,
 *          as well as a reference to the bank this element belongs to.
 * @tparam  Sender
 *          The MIDI Sender class.
 * @tparam  NumRows 
 *          The number of rows of the button matrix.
 * @tparam  NumCols
 *          The number of columns of the button matrix.
 */
template <class BankAddress, class Sender, uint8_t NumRows, uint8_t NumCols>
class MIDIButtonMatrix
    : public MIDIOutputElement,
      public AH::ButtonMatrix<
          MIDIButtonMatrix<BankAddress, Sender, NumRows, NumCols>, NumRows,
          NumCols> {
    using ButtonMatrix = AH::ButtonMatrix<MIDIButtonMatrix, NumRows, NumCols>;
    friend class AH::ButtonMatrix<MIDIButtonMatrix, NumRows, NumCols>;

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
    MIDIButtonMatrix(BankAddress bankAddress, const PinList<NumRows> &rowPins,
                     const PinList<NumCols> &colPins, const Sender &sender)
        : ButtonMatrix(rowPins, colPins), address(bankAddress), sender(sender) {
    }

  public:
    void begin() override { ButtonMatrix::begin(); }

    void update() override { ButtonMatrix::update(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) {
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

  protected:
    BankAddress address;
    uint16_t activeButtons = 0;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE