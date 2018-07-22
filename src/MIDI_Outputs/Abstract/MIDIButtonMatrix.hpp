#pragma once

#include <Hardware/ButtonMatrix.h>
#include <Helpers/Array.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

template <uint8_t nb_rows, uint8_t nb_cols>
using AddressMatrix = Array2D<uint8_t, nb_rows, nb_cols>;

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     ButtonMatrix
 */
template <class Sender, uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public MIDIOutputElement,
                         public ButtonMatrix<nb_rows, nb_cols> {

  protected:
    /**
     * @brief
     * @todo    Documentation
     */
    MIDIButtonMatrix(const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     uint8_t channel)
        : ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins),
          addresses(addresses), baseChannel(channel) {}

  public:
    void update() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        uint8_t address = addresses[row][col];
        if (state == LOW)
            Sender::sendOn(baseChannel, address);
        else
            Sender::sendOff(baseChannel, address);
    }

    AddressMatrix<nb_rows, nb_cols> addresses;
    const uint8_t baseChannel;
};