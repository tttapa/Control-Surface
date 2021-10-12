#pragma once

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Def/Def.hpp>
#include <Selectors/Selector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class SelectorLEDsCallback {
  public:
    SelectorLEDsCallback(const PinList<N> &ledPins) : ledPins(ledPins) {}

    /// Initialize.
    void begin() {
        for (pin_t pin : ledPins) {
            AH::ExtIO::pinMode(pin, OUTPUT);
            AH::ExtIO::digitalWrite(pin, LOW);
        }
    }

    /// Refresh, called periodically.
    void update() {}

    /// Called when the setting changes.
    void update(setting_t oldSetting, setting_t newSetting) {
        AH::ExtIO::digitalWrite(ledPins[oldSetting], LOW);
        AH::ExtIO::digitalWrite(ledPins[newSetting], HIGH);
    }

  private:
    PinList<N> ledPins;
};

class SelectorSingleLEDCallback {
  public:
    SelectorSingleLEDCallback(pin_t ledPin) : ledPin(ledPin) {}

    /// Initialize.
    void begin() { AH::ExtIO::pinMode(ledPin, OUTPUT); }

    /// Refresh, called periodically.
    void update() {}

    /// Called when the setting changes.
    void update(setting_t oldSetting, setting_t newSetting) {
        (void)oldSetting;
        AH::ExtIO::digitalWrite(ledPin, newSetting == 0 ? LOW : HIGH);
    }

  private:
    pin_t ledPin;
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#include <Selectors/EncoderSelector.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   @copybrief EncoderSelector
 *          This version displays the setting using LEDs.
 * @details @copydetails EncoderSelector
 * @ingroup SelectorsLEDs
 */
template <setting_t N>
class EncoderSelectorLEDs
    : public GenericEncoderSelector<N, SelectorLEDsCallback<N>> {
  public:
    EncoderSelectorLEDs(Selectable<N> &selectable,
                        const EncoderSwitchPinList &pins,
                        const PinList<N> &ledPins, int8_t pulsesPerStep = 4,
                        Wrap wrap = Wrap::Wrap)
        : GenericEncoderSelector<N, SelectorLEDsCallback<N>>{
              selectable, {ledPins}, pins, pulsesPerStep, wrap,
          } {}
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#include <Selectors/IncrementDecrementSelector.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   @copybrief IncrementDecrementSelector
 *          This version displays the setting using LEDs.
 * @details @copydetails IncrementDecrementSelector
 * @ingroup SelectorsLEDs
 */
template <setting_t N>
class IncrementDecrementSelectorLEDs
    : public GenericIncrementDecrementSelector<N, SelectorLEDsCallback<N>> {
  public:
    IncrementDecrementSelectorLEDs(Selectable<N> &selectable,
                                   const AH::IncrementDecrementButtons &buttons,
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

/**
 * @brief   @copybrief IncrementSelector
 *          This version displays the setting using LEDs.
 * @details @copydetails IncrementSelector
 * @ingroup SelectorsLEDs
 */
template <setting_t N>
class IncrementSelectorLEDs
    : public GenericIncrementSelector<N, SelectorLEDsCallback<N>> {
  public:
    IncrementSelectorLEDs(Selectable<N> &selectable, const AH::Button &button,
                          const PinList<N> &ledPins)
        : GenericIncrementSelector<N, SelectorLEDsCallback<N>>{
              selectable, {ledPins}, button} {}
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#include <Selectors/ManyButtonsSelector.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   @copybrief ManyButtonsSelector
 *          This version displays the setting using LEDs.
 * @details @copydetails ManyButtonsSelector
 * @ingroup SelectorsLEDs
 */
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

/**
 * @brief   @copybrief ProgramChangeSelector
 *          This version displays the setting using LEDs.
 * @details @copydetails ProgramChangeSelector
 * @ingroup SelectorsLEDs
 */
template <setting_t N>
class ProgramChangeSelectorLEDs
    : public GenericProgramChangeSelector<N, SelectorLEDsCallback<N>> {
  public:
    ProgramChangeSelectorLEDs(Selectable<N> &selectable,
                              MIDIChannelCable address,
                              const PinList<N> &ledPins)
        : GenericProgramChangeSelector<N, SelectorLEDsCallback<N>>{
              selectable,
              {ledPins},
              address,
          } {}
};

END_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#include <Selectors/SwitchSelector.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   @copybrief SwitchSelector
 *          This version displays the setting using LEDs.
 * @details @copydetails SwitchSelector
 * @ingroup SelectorsLEDs
 */
class SwitchSelectorLED
    : public GenericSwitchSelector<SelectorSingleLEDCallback> {
  public:
    SwitchSelectorLED(Selectable<2> &selectable, const AH::Button &button,
                      pin_t ledPin)
        : GenericSwitchSelector<SelectorSingleLEDCallback>{
              selectable,
              {ledPin},
              button,
          } {}
};

END_CS_NAMESPACE