#pragma once

#include <Selectors/Selector.h>
#include <Banks/Bank.h>

/**
 * @brief   A class for changing the setting of a Bank
 *          depending on the selection of a Selector.
 *
 * Can be used for selecting fader banks,
 * changing the channel of MIDI_Control_Element%s,
 * transposing notes, displaying different tracks
 * for MIDIInputElement%s, etc.
 *
 * Refer to the documentation of Selector for information
 * on the different modes of operation.
 */
class BankSelector : public Selector {
  public:
    /**
     * @see Selector::Selector
     */
    template <class... Args>
    BankSelector(Bank &bank, Args... args) : Selector(args...), bank(bank) {}

    // Initializer_lists are not supported with variadic templates, so overload
    // them manually

    /**
     * @see Selector::Selector
     */
    // Multiple buttons, no LEDs
    template <class T, size_t N>
    BankSelector(Bank &bank, const T (&switchPins)[N])
        : Selector((const pin_t (&)[N])switchPins), bank(bank) {}

    /**
     * @see Selector::Selector
     */
    // One (+1), two (+1, -1) or multiple buttons, multiple LEDs
    template <class T, class S, size_t N, size_t M>
    BankSelector(Bank &bank, const T (&switchPins)[N], const S (&ledPins)[M])
        : Selector((const pin_t (&)[N])switchPins, (const pin_t (&)[M])ledPins),
          bank(bank) {}

    /**
     * @see Selector::Selector
     */
    // One (+1) or two (+1, -1) buttons, no LEDs
    template <class T, class S, size_t N>
    BankSelector(Bank &bank, const T (&switchPins)[N], S nb_settings)
        : Selector((const pin_t (&)[N])switchPins, (size_t)nb_settings),
          bank(bank) {}

  private:
    Bank &bank;
    /**
     * @brief   Update the bank setting with the new selector setting.
     */
    void updateImpl(uint8_t newSetting) { bank.setBankSetting(newSetting); }
};
