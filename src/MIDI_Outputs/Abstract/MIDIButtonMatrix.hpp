#pragma once

#include <AH/Hardware/ButtonMatrix.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     AH::ButtonMatrix
 */
template <class Sender, uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public MIDIOutputElement,
                         public AH::ButtonMatrix<nb_rows, nb_cols> {

  protected:
    /**
     * @brief
     * @todo    Documentation
     */
    MIDIButtonMatrix(const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     MIDICNChannel channelCN, const Sender &sender)
        : AH::ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins),
          addresses(addresses), baseChannelCN(channelCN), sender{sender} {}

  public:
    void begin() final override { AH::ButtonMatrix<nb_rows, nb_cols>::begin(); }

    void update() final override {
        AH::ButtonMatrix<nb_rows, nb_cols>::update();
    }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        int8_t address = addresses[row][col];
        if (state == LOW)
            sender.sendOn({address, baseChannelCN});
        else
            sender.sendOff({address, baseChannelCN});
    }

    AddressMatrix<nb_rows, nb_cols> addresses;
    const MIDICNChannel baseChannelCN;

  public:
    Sender sender;
};

END_CS_NAMESPACE