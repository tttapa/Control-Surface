#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <MIDI_Inputs/NoteCCRange.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Callback class that drives a LED dot/bar display based on a note or
 *          control change value.
 * 
 * @tparam  NumLEDs 
 *          The number of LEDs the display has.
 */
template <uint8_t NumLEDs>
class NoteCCLEDBarCallback : public SimpleNoteCCValueCallback {
  public:
    NoteCCLEDBarCallback(const AH::DotBarDisplayLEDs<NumLEDs> &leds)
        : leds(leds) {}

    void begin(const INoteCCValue &t) override {
        leds.begin();
        updateAll(t);
    }

    void update(const INoteCCValue &t, uint8_t) override {
        uint8_t value = t.getValue();
        leds.display(value / 127.0f);
    }

    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { leds.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { leds.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { leds.setMode(mode); }

  private:
    AH::DotBarDisplayLEDs<NumLEDs> leds;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/**
 * @brief   Class that listens for **Note** events and displays the velocity on
 *          an **LED Bar Graph**.
 * 
 * @tparam  NumLEDs 
 *          The number of LEDs the display has.
 * @ingroup midi-input-elements-leds
 */
template <uint8_t NumLEDs>
class NoteLEDBar : public GenericNoteCCRange<MIDIInputElementNote, 1,
                                             NoteCCLEDBarCallback<NumLEDs>> {
  public:
    NoteLEDBar(const AH::DotBarDisplayLEDs<NumLEDs> &leds,
               MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, 1,
                             NoteCCLEDBarCallback<NumLEDs>>{
              address,
              {leds},
          } {}

    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { this->callback.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { this->callback.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { this->callback.setMode(mode); }
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/**
 * @brief   Class that listens for **Control Change** events and displays the
 *          velocity on an **LED Bar Graph**.
 * 
 * @tparam  NumLEDs 
 *          The number of LEDs the display has.
 * @ingroup midi-input-elements-leds
 */
template <uint8_t NumLEDs>
class CCLEDBar : public GenericNoteCCRange<MIDIInputElementCC, 1,
                                           NoteCCLEDBarCallback<NumLEDs>> {
  public:
    CCLEDBar(const AH::DotBarDisplayLEDs<NumLEDs> &leds,
             MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, 1,
                             NoteCCLEDBarCallback<NumLEDs>>{
              address,
              {leds},
          } {}

    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { this->callback.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { this->callback.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { this->callback.setMode(mode); }
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

namespace Bankable {

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/**
 * @brief   Class that listens for **Note** events and displays the velocity on
 *          an **LED Bar Graph**.
 * 
 * This version can be banked.
 * 
 * @tparam  NumLEDs 
 *          The number of LEDs the display has.
 * @ingroup midi-input-elements-leds
 */
template <uint8_t BankSize, uint8_t NumLEDs>
class NoteLEDBar : public GenericNoteCCRange<MIDIInputElementNote, 1, BankSize,
                                             NoteCCLEDBarCallback<NumLEDs>> {
  public:
    NoteLEDBar(const BankConfig<BankSize> &config,
               const AH::DotBarDisplayLEDs<NumLEDs> &leds,
               const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementNote, 1, BankSize,
                             NoteCCLEDBarCallback<NumLEDs>>{
              config,
              address,
              {leds},
          } {}

    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { this->callback.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { this->callback.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { this->callback.setMode(mode); }
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/**
 * @brief   Class that listens for **Control Change** events and displays the
 *          velocity on an **LED Bar Graph**.
 * 
 * This version can be banked.
 * 
 * @tparam  NumLEDs 
 *          The number of LEDs the display has.
 * @ingroup midi-input-elements-leds
 */
template <uint8_t BankSize, uint8_t NumLEDs>
class CCLEDBar : public GenericNoteCCRange<MIDIInputElementCC, 1, BankSize,
                                           NoteCCLEDBarCallback<NumLEDs>> {
  public:
    CCLEDBar(const BankConfig<BankSize> &config,
             const AH::DotBarDisplayLEDs<NumLEDs> &leds,
             const MIDICNChannelAddress &address)
        : GenericNoteCCRange<MIDIInputElementCC, 1, BankSize,
                             NoteCCLEDBarCallback<NumLEDs>>{
              config,
              address,
              {leds},
          } {}

    /// @copydoc    AH::DotBarDisplayLEDs::dotMode
    void dotMode() { this->callback.dotMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::barMode
    void barMode() { this->callback.barMode(); }
    /// @copydoc    AH::DotBarDisplayLEDs::setMode
    void setMode(AH::DotBarMode mode) { this->callback.setMode(mode); }
};

} // namespace Bankable

END_CS_NAMESPACE