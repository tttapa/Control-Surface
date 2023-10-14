#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonMatrix.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the input from a **matrix
 *          of momentary push buttons or switches**, and send out MIDI **Note**
 *          events.
 * 
 * A Note On event is sent when a button is pressed, and a Note Off
 * event is sent when a button is released.  
 * Crude software debouncing is implemented by limiting the refresh rate.  
 * This version can be banked using an arbitrary list of alternative
 * addresses.
 *          
 * @tparam  NumBanks
 *          The number of variants/alternative addresses the element has.
 * @tparam  NumRows
 *          The number of rows of the matrix.
 * @tparam  NumCols
 *          The number of columns of the matrix.
 * 
 * @ingroup ManyAddressesMIDIOutputElements
 */
template <setting_t NumBanks, uint8_t NumRows, uint8_t NumCols>
class NoteButtonMatrix
    : public MIDIButtonMatrix<ManyMatrixAddresses<NumBanks, NumRows, NumCols>,
                              DigitalNoteSender, NumRows, NumCols> {
  public:
    /**
     * @brief   Create a new Bankable NoteButtonMatrix object with the given 
     *          pins, controller numbers and channel.
     * 
     * @param   bank
     *          The bank to add this element to.
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW as outputs (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix.  
     *          These pins will be used as inputs (Hi-Z), and the
     *          internal pull-up resistor will be enabled.
     * @param   notes
     *          A list of 2-dimensional arrays of the same dimensions as the 
     *          button matrix that contains the MIDI Note number of each button.
     *          [0, 127]
     * @param   channelCNs
     *          The a list containing the MIDI channels [Channel_1, Channel_16] 
     *          and Cable Numbers [Cable_1, Cable_16].
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    NoteButtonMatrix(
        const Bank<NumBanks> &bank, const PinList<NumRows> &rowPins,
        const PinList<NumCols> &colPins,
        const Array<AddressMatrix<NumRows, NumCols>, NumBanks> &notes,
        const Array<MIDIChannelCable, NumBanks> &channelCNs,
        uint8_t velocity = 0x7F)
        : MIDIButtonMatrix<ManyMatrixAddresses<NumBanks, NumRows, NumCols>,
                           DigitalNoteSender, NumRows, NumCols> {
              {bank, notes, channelCNs},
              rowPins,
              colPins,
              {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE