#pragma once

#include <Def/Def.hpp>
#include <Hardware/ButtonMatrix.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     ButtonMatrix
 */
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff,
          uint8_t nb_rows, uint8_t nb_cols>
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
                     MIDICNChannel channelCN)
        : ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins),
          addresses(addresses), baseChannelCN(channelCN) {}

  public:
    void begin() final override { ButtonMatrix<nb_rows, nb_cols>::begin(); }

    void update() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        int8_t address = addresses[row][col];
        if (state == LOW)
            sendOn({address, baseChannelCN});
        else
            sendOff({address, baseChannelCN});
    }

    AddressMatrix<nb_rows, nb_cols> addresses;
    const MIDICNChannel baseChannelCN;
};