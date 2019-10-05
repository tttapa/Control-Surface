#pragma once

#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Selectors/Selector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class SelectorLEDsCallback {
  public:
    SelectorLEDsCallback(const PinList<N> &ledPins) : ledPins{ledPins} {}

    /// Initialize.
    void begin() {
        for (const pin_t &pin : ledPins) {
            ExtIO::pinMode(pin, OUTPUT);
            ExtIO::digitalWrite(pin, LOW);
        }
        ExtIO::digitalWrite(ledPins[this->get()], HIGH);
    }

    /// Refresh, called periodically.
    void update() {}

    /// Called when the setting changes.
    void update(setting_t oldSetting, setting_t newSetting) {
        ExtIO::digitalWrite(ledPins[oldSetting], LOW);
        ExtIO::digitalWrite(ledPins[newSetting], HIGH);
    }

  private:
    const PinList<N> ledPins;
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#if defined(Encoder_h_) || defined(IDE)
#include <Selectors/EncoderSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class EncoderSelectorLEDs
    : public GenericEncoderSelector<N, SelectorLEDsCallback<N>> {
  public:
    EncoderSelectorLEDs(Selectable<N> &selectable, const PinList<N> &ledPins,
                        const EncoderSwitchPinList &pins,
                        uint8_t pulsesPerStep = 4, Wrap wrap = Wrap::Wrap)
        : GenericEncoderSelector<N, SelectorLEDsCallback<N>>{
              selectable, {ledPins}, pins, pulsesPerStep, wrap,
          } {}
};

END_CS_NAMESPACE
#endif

// -------------------------------------------------------------------------- //

#include <Selectors/IncrementDecrementSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class IncrementDecrementSelectorLEDs
    : public GenericIncrementDecrementSelector<N, SelectorLEDsCallback<N>> {
  public:
    IncrementDecrementSelectorLEDs(Selectable<N> &selectable,
                                   const IncrementDecrementButtons &buttons,
                                   const PinList<N> &ledPins,
                                   Wrap wrap = Wrap::Wrap)
        : GenericIncrementDecrementSelector<N, SelectorLEDsCallback<N>>{
              selectable,
              {ledPins},
              buttons,
              wrap,
          } {}
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#include <Selectors/IncrementSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class IncrementSelectorLEDs
    : public GenericIncrementSelector<N, SelectorLEDsCallback<N>> {
  public:
    IncrementSelectorLEDs(Selectable<N> &selectable, const Button &button,
                          const PinList<N> &ledPins, Wrap wrap = Wrap::Wrap)
        : GenericIncrementSelector<N, SelectorLEDsCallback<N>>{
              selectable,
              {ledPins},
              button,
              wrap,
          } {}
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#include <Selectors/ManyButtonsSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class ManyButtonsSelectorLEDs
    : public GenericManyButtonsSelector<N, SelectorLEDsCallback<N>> {
  public:
    ManyButtonsSelectorLEDs(Selectable<N> &selectable,
                            const PinList<N> &buttonPins,
                            const PinList<N> &ledPins)
        : GenericManyButtonsSelector<N, SelectorLEDsCallback<N>>{
              selectable,
              {ledPins},
              buttonPins,
          } {}
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#include <Selectors/ProgramChangeSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class ProgramChangeSelectorLEDs
    : public GenericProgramChangeSelector<N, SelectorLEDsCallback<N>> {
  public:
    ProgramChangeSelectorLEDs(Selectable<N> &selectable,
                              const MIDICNChannel &address,
                              const PinList<N> &ledPins)
        : GenericProgramChangeSelector<N, SelectorLEDsCallback<N>>{
              selectable,
              {ledPins},
              address,
          } {}
};

END_CS_NAMESPACE