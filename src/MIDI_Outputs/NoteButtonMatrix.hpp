#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonMatrix.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

template <uint8_t nb_rows, uint8_t nb_cols>
class NoteButtonMatrix
    : public MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols> {
  public:
    NoteButtonMatrix(const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     uint8_t channel)
        : MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols>(
              rowPins, colPins, addresses, channel) {}
};

namespace Bankable {

template <uint8_t nb_rows, uint8_t nb_cols>
class NoteButtonMatrix
    : public MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols> {
  public:
    NoteButtonMatrix(const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     uint8_t channel)
        : MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols>(
              rowPins, colPins, addresses, channel) {}
};

} // namespace Bankable
