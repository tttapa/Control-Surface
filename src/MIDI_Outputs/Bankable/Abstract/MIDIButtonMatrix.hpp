#pragma once

#include <Banks/BankableMIDIOutputAddressable.hpp>
#include <Hardware/ButtonMatrix.h>
#include <Helpers/Array.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#include <MIDI_Outputs/Abstract/MIDIButtonMatrix.hpp> // For AddressMatrix

namespace Bankable {

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     ButtonMatrix
 */
template <class Sender, uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public BankableMIDIOutputAddressable,
                         public MIDIOutputElement,
                         public ButtonMatrix<nb_rows, nb_cols> {

  protected:
    /**
     * @brief
     * @todo    Documentation
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
    void update() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        uint8_t address = getAddress(addresses[row][col]);
        if (state == LOW) {
            if (!activeButtons)
                lock(); // Don't allow changing of the bank setting
            activeButtons++;
            Sender::sendOn(getChannel(baseChannel), address);
        } else {
            Sender::sendOff(getChannel(baseChannel), address);
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