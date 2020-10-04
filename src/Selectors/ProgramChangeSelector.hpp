#pragma once

#include <MIDI_Inputs/MIDIInputElement.hpp>
#include <MIDI_Inputs/MIDIInputElementMatchers.hpp>
#include <Selectors/Selector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericProgramChangeSelector
    : public GenericSelector<N, Callback>,
      public MatchingMIDIInputElement<MIDIMessageType::PROGRAM_CHANGE,
                                      OneByteMIDIMatcher> {
  public:
    using Matcher = OneByteMIDIMatcher;
    using Parent = MatchingMIDIInputElement<MIDIMessageType::PROGRAM_CHANGE,
                                            Matcher>;

    GenericProgramChangeSelector(Selectable<N> &selectable,
                                 const Callback &callback,
                                 MIDIChannelCable address)
        : GenericSelector<N, Callback>{selectable, callback},
          Parent(address) {}

    void begin() override { GenericSelector<N, Callback>::begin(); }

    void reset() override { GenericSelector<N, Callback>::reset(); }

    void handleUpdate(typename Matcher::Result match) override {
        uint8_t program = match.value;
        if (program < N) {
            this->set(program);
        } else {
            DEBUGFN(F("Warning: Received Program Change to program 0x")
                    << hex << program << dec
                    << F(", which is not smaller than the number of settings (")
                    << N << ')');
        }
    }
};

/**
 * @brief   Selector that listens for MIDI Program Change events on a given
 *          MIDI Channel, and uses the program number as its selection.
 * 
 * @tparam  N 
 *          The number of settings. The maximum program number is @f$ N - 1 @f$.
 * @ingroup Selectors
 */
template <setting_t N>
class ProgramChangeSelector : public GenericProgramChangeSelector<N> {
  public:
    ProgramChangeSelector(Selectable<N> &selectable,
                          MIDIChannelCable address)
        : GenericProgramChangeSelector<N>{selectable, {}, address} {}
};

END_CS_NAMESPACE