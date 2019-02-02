#include "MIDIInputElementPC.hpp"
#include <Selectors/LEDs/SelectorLEDs.hpp>

template <setting_t N>
class ProgramChangeSelector_Base : public MIDIInputElementPC,
                                   virtual public Selector<N> {
  public:
    ProgramChangeSelector_Base(const MIDICNChannel &address)
        : MIDIInputElementPC(address) {}

    void begin() override {
        MIDIInputElementPC::begin();
        Selector<N>::begin();
    }

    void beginInput() override {}

    void reset() override { Selector<N>::reset(); }

    void update() override {}

    bool updateImpl(const MIDI_message_matcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        (void)target;
        uint8_t program = midimsg.data1;
        if (program < N)
            this->set(program);
        else
            DEBUGFN(F("Warning: Received Program Change to program 0x")
                    << hex << +program << dec
                    << F(", which is not smaller than the number of settings (")
                    << +N << ')');
        return true;
    }
};

template <setting_t N>
class ProgramChangeSelector : public ProgramChangeSelector_Base<N> {
  public:
    ProgramChangeSelector(Selectable<N> &selectable,
                          const MIDICNChannel &address)
        : Selector<N>{selectable}, ProgramChangeSelector_Base<N>{address} {}

    void beginOutput() override {}
    void updateOutput(UNUSED_PARAM setting_t oldSetting,
                      UNUSED_PARAM setting_t newSetting) override {}
};

template <setting_t N>
class ProgramChangeSelectorLEDs : public ProgramChangeSelector_Base<N>,
                                  public SelectorLEDs<N> {
  public:
    ProgramChangeSelectorLEDs(Selectable<N> &selectable,
                              const MIDICNChannel &address,
                              const PinList<N> &ledPins)
        : Selector<N>{selectable}, ProgramChangeSelector_Base<N>{address},
          SelectorLEDs<N>{ledPins} {}
};