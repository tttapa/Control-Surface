#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonMatrix.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the input from a **matrix
 *          of momentary push buttons or switches**, and send out MIDI **Control
 *          Change** events.
 * 
 * A value of 0x7F is sent when a button is pressed, and a value of 0x00 is sent
 * when a button is released.  
 * Crude software debouncing is implemented by limiting the refresh rate.  
 *
 *  * This version can be banked using an arbitrary list of alternative
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
class CCButtonMatrix
    : public MIDIButtonMatrix<ManyMatrixAddresses<NumBanks, NumRows, NumCols>,
                              DigitalCCSender, NumRows, NumCols> {
  public:
    /**
     * @brief   Create a new Bankable CCButtonMatrix object with the given pins,
     *          controller numbers and channel.
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
     * @param   controllers
     *          A list of 2-dimensional arrays of the same dimensions as the 
     *          button matrix that contains the MIDI Controller number of each 
     *          button.
     *          [0, 119]
     * @param   channelCNs
     *          The a list containing the MIDI channels [CHANNEL_1, CHANNEL_16] 
     *          and Cable Numbers [CABLE_1, CABLE_16].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtonMatrix(
        const Bank<NumBanks> &bank, const PinList<NumRows> &rowPins,
        const PinList<NumCols> &colPins,
        const Array<AddressMatrix<NumRows, NumCols>, NumBanks> &controllers,
        const Array<MIDIChannelCable, NumBanks> &channelCNs,
        const DigitalCCSender &sender = {})
        : MIDIButtonMatrix<ManyMatrixAddresses<NumBanks, NumRows, NumCols>,
                           DigitalCCSender, NumRows, NumCols>{
              {bank, controllers, channelCNs}, rowPins, colPins, sender} {}
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE