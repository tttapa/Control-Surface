#pragma once

#include <MIDI_Inputs/MIDIInputElementPC.hpp>
#include <Selectors/Selector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericProgramChangeSelector : public GenericSelector<N, Callback>,
                                     public MIDIInputElementPC {
  public:
    GenericProgramChangeSelector(Selectable<N> &selectable,
                                 const Callback &callback,
                                 const MIDICNChannel &address)
        : GenericSelector<N, Callback>{selectable, callback},
          MIDIInputElementPC{address} {}

    void begin() override {
        MIDIInputElementPC::begin();
        GenericSelector<N, Callback>::begin();
    }

    void reset() override { GenericSelector<N, Callback>::reset(); }

    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        (void)target;
        uint8_t program = midimsg.data1;
        if (program < N) {
            this->set(program);
        } else {
            DEBUGFN(F("Warning: Received Program Change to program 0x")
                    << hex << program << dec
                    << F(", which is not smaller than the number of settings (")
                    << N << ')');
        }
        return true;
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
                          const MIDICNChannel &address)
        : GenericProgramChangeSelector<N>{selectable, {}, address} {}
};

END_CS_NAMESPACE