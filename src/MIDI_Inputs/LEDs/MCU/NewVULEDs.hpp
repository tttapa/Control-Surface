#pragma once

#include <AH/Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <MIDI_Inputs/MCU/NewVU.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/// Small helper to display the VU meters on an LED bar display.
template <uint8_t NumLEDs>
class VULEDsDriver : public AH::DotBarDisplayLEDs<NumLEDs> {
  public:
    /// Constructor
    VULEDsDriver(const PinList<NumLEDs> &leds)
        : AH::DotBarDisplayLEDs<NumLEDs>(leds) {}

    /// @param  value
    ///         [0, 12]
    void displayVU(uint8_t value) {
        value = (value * NumLEDs + FLOOR_CORRECTION) / 12; // [0, N]
        this->display(value);
    }

  private:
    /// @see    doc/VU-LED-mapping.ods
    constexpr static uint8_t FLOOR_CORRECTION = 5;
};

// -------------------------------------------------------------------------- //

template <uint8_t NumLEDs>
class NewVULEDs : public NewVU, public VULEDsDriver<NumLEDs> {
  public:
    /// Constructor.
    ///
    /// @param  track
    ///         The track to listen for [1, 8].
    NewVULEDs(const PinList<NumLEDs> &leds, uint8_t track,
              MIDIChannelCN channel, unsigned int decayTime = VUDecay::Default)
        : NewVU(track, channel, decayTime), VULEDsDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(VUMatcher::Result match) override {
        NewVU::handleUpdate(match);
        updateDisplay();
    }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayVU(getValue());
            clearDirty();
        }
    }

  public:
    void begin() override {
        NewVU::begin();
        VULEDsDriver<NumLEDs>::begin();
    }

    void reset() override {
        NewVU::reset();
        updateDisplay();
    }

    void update() override {
        NewVU::update();
        updateDisplay();
    }

  protected:
    using NewVU::clearDirty;
    using NewVU::getDirty;
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/// @param  BankSize
///         The number of banks.
template <uint8_t BankSize, uint8_t NumLEDs>
class NewVULEDs : public NewVU<BankSize>, public VULEDsDriver<NumLEDs> {
  public:
    /// Constructor.
    ///
    /// @param  config
    ///         The bank configuration to use.
    /// @param  track
    ///         The track to listen for [1, 8].
    /// @param  channel
    ///         The MIDI channel and cable.
    NewVULEDs(BankConfig<BankSize> config, const PinList<NumLEDs> &leds,
              uint8_t track, MIDIChannelCN channel,
              unsigned int decayTime = VUDecay::Default)
        : NewVU<BankSize>(config, track, channel, decayTime),
          VULEDsDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(typename VUMatcher<BankSize>::Result match) override {
        NewVU<BankSize>::handleUpdate(match);
        if (match.bankIndex == getActiveBank())
            updateDisplay();
    }

    uint8_t getActiveBank() const { return this->matcher.getSelection(); }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayVU(this->getValue());
            clearDirty();
        }
    }

  public:
    void begin() override {
        NewVU<BankSize>::begin();
        VULEDsDriver<NumLEDs>::begin();
    }

    void reset() override {
        NewVU<BankSize>::reset();
        updateDisplay();
    }

    void update() override {
        NewVU<BankSize>::update();
        updateDisplay();
    }

  private:
    void onBankSettingChange() override { this->displayVU(this->getValue()); }

  protected:
    using NewVU<BankSize>::clearDirty;
    using NewVU<BankSize>::getDirty;
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE