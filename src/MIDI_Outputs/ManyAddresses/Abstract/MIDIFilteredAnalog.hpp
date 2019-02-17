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
template <setting_t N, ContinuousSendFunction7Bit send, uint8_t PRECISION>
class MIDIFilteredAnalogAddressable : public MIDIOutputElementPotentiometer,
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
        const Array<MIDICNChannelAddress, N> &addresses)
        : ManyAddressesMIDIOutput<N>{bank},
          filteredAnalog{analogPin}, addresses{addresses} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue(), addresses[this->getSelection()]);
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the 10-bit analog value as a parameter, and
     *          should return a 10-bit value.
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

  private:
    FilteredAnalog<PRECISION> filteredAnalog;
    const Array<MIDICNChannelAddress, N> addresses;
};

} // namespace ManyAddresses