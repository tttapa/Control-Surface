#include <Banks/ManyAddressesMIDIOuptut.hpp>

namespace ManyAddresses {

/**
 * @brief   A class for potentiometers and faders that send MIDI events and that
 *          have different addresses that can be selected.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @tparam  PRECISION
 *          The analog precision in bits.
 *
 * @see     FilteredAnalog
 */
template <setting_t N, class Sender, uint8_t PRECISION>
class MIDIFilteredAnalogAddressable : public MIDIOutputElement,
                                      public ManyAddressesMIDIOutput<N> {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalogAddressable.
     *
     * @param   bank
     *          The bank that selects the address to use.
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @todo    Documentation.
     */
    MIDIFilteredAnalogAddressable(
        const Bank<N> &bank, pin_t analogPin,
        const Array<MIDICNChannelAddress, N> &addresses, const Sender &sender)
        : ManyAddressesMIDIOutput<N>{bank},
          filteredAnalog{analogPin}, addresses{addresses}, sender{sender} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            sender.send(filteredAnalog.getValue(),
                        addresses[this->getSelection()]);
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of 10 bits as a 
     *          parameter, and should return a value of 10 bits.
     * 
     * @see     FilteredAnalog::map     
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

    /**
     * @brief   Get the raw value of the analog input (this is the value 
     *          without applying the mapping function first).
     */
    uint8_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    uint8_t getValue() const { return filteredAnalog.getValue(); }

  private:
    FilteredAnalog<PRECISION> filteredAnalog;
    const Array<MIDICNChannelAddress, N> addresses;
    Sender sender;
};

} // namespace ManyAddresses