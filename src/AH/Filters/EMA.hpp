/* ✔ */

#pragma once

#include <stdint.h>
#include <AH/STL/limits>
#include <AH/STL/type_traits>

#include <AH/Settings/NamespaceSettings.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   Exponential moving average filter.
 * 
 * Fast integer EMA implementation where the weight factor is a power of two.
 * 
 * Difference equation: @f$ y[n] = \alpha·x[n]+(1-\alpha)·y[n-1] @f$
 * where @f$ \alpha = \left(\frac{1}{2}\right)^{K} @f$, @f$ x @f$ is the
 * input sequence, and @f$ y @f$ is the output sequence.
 *
 * [An in-depth explanation of the EMA filter](https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/)
 *
 * @tparam  K
 *          The amount of bits to shift by. This determines the location
 *          of the pole in the EMA transfer function, and therefore the
 *          cut-off frequency.  
 *          The higher this number, the more filtering takes place.  
 *          The pole location is @f$ 1 - 2^{-K} @f$.
 * @tparam  input_t
 *          The integer type to use for the input and output of the filter. 
 *          Can be signed or unsigned.
 * @tparam  state_t
 *          The unsigned integer type to use for the internal state of the
 *          filter. A fixed-point representation with @f$ K @f$ fractional
 *          bits is used, so this type should be at least @f$ M + K @f$ bits
 *          wide, where @f$ M @f$ is the maximum number of bits of the input.
 *
 * Some examples of different combinations of template parameters:
 * 
 * 1. Filtering the result of `analogRead`: analogRead returns an integer
 *    between 0 and 1023, which can be represented using 10 bits, so 
 *    @f$ M = 10 @f$. If `input_t` and `output_t` are both `uint16_t`,
 *    the maximum shift factor `K` is @f$ 16 - M = 6 @f$. If `state_t`
 *    is increased to `uint32_t`, the maximum shift factor `K` is 
 *    @f$ 32 - M = 22 @f$.
 * 2. Filtering a signed integer between -32768 and 32767: this can be 
 *    represented using a 16-bit signed integer, so `input_t` is `int16_t`,
 *    and @f$ M = 16 @f$. (2¹⁵ = 32768)
 *    Let's say the shift factor `K` is 1, then the minimum width of 
 *    `state_t` should be @f$ M + K = 17 @f$ bits, so `uint32_t` would be 
 *    a sensible choice.
 * 
 * @ingroup    AH_Filters
 */
template <uint8_t K,
          class input_t = uint_fast16_t,
          class state_t = typename std::make_unsigned<input_t>::type>
class EMA {
  public:
    /// Constructor: initialize filter to zero or optional given value.
    EMA(input_t initial = input_t{0}) { reset(initial); }

    /**
     * @brief   Reset the filter to the given value.
     * 
     * @param   value 
     *          The value to reset the filter state to.
     */
    void reset(input_t value = input_t(0)) {
        state_t value_s = static_cast<state_t>(value);
        state = zero + (value_s << K) - value_s;
    }

    /**
     * @brief   Filter the input: Given @f$ x[n] @f$, calculate @f$ y[n] @f$.
     *
     * @param   input
     *          The new raw input value.
     * @return  The new filtered output value.
     */
    input_t filter(input_t input) {
      state         += static_cast<state_t>(input);
      state_t output = (state + half) >> K;
      output        -= zero >> K;
      state         -= output;
      return static_cast<input_t>(output);
    }

    /// @copydoc    EMA::filter(input_t)
    input_t operator()(input_t input) {
        return filter(input);
    }

    constexpr static state_t 
      max_state  = std::numeric_limits<state_t>::max(),
      half_state = max_state / 2 + 1,
      zero       = std::is_unsigned<input_t>::value ? state_t{0} : half_state,
      half       = K > 0 ? state_t{1} << (K - 1) : state_t{0};
  
    static_assert(std::is_unsigned<state_t>::value, 
                  "state type should be unsigned");

    static_assert(max_state >= std::numeric_limits<input_t>::max(),
                  "state type cannot be narrower than input type");               

    /// Verify the input range to make sure it's compatible with the shift
    /// factor and the width of the state type.
    ///
    /// Examples:
    /// ~~~cpp
    /// EMA<5, int_fast16_t, uint_fast16_t> filter;
    /// static_assert(filter.supports_range(-1024, 1023),
    ///               "use a wider state or input type, or a smaller shift factor");
    /// ~~~
    /// ~~~cpp
    /// EMA<5, uint_fast16_t, uint_fast16_t> filter;
    /// static_assert(filter.supports_range(0u, 2047u),
    ///               "use a wider state or input type, or a smaller shift factor");
    /// ~~~
    template <class T>
    constexpr static bool supports_range(T min, T max) {
      using sstate_t = typename std::make_signed<state_t>::type;
      return min <= max &&
             min >= std::numeric_limits<input_t>::min() &&
             max <= std::numeric_limits<input_t>::max() &&
             (std::is_unsigned<input_t>::value
               ? state_t(max) <= (max_state >> K)
               : min >= -static_cast<sstate_t>(max_state >> (K + 1)) - 1 &&  
                 max <= static_cast<sstate_t>(max_state >> (K + 1)));
    }

  private:
    state_t state;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for single-pole infinite impulse response filters
 *          or exponential moving average filters.
 * 
 * This version uses floating point maths.
 * 
 * Difference equation: @f$ y[n] = \alpha·x[n]+(1-\alpha)·y[n-1] @f$
 * @f$ x @f$ is the input sequence, and @f$ y @f$ is the output sequence.
 *
 * [An in-depth explanation of the EMA filter]
 * (https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/)
 * 
 * @ingroup    AH_Filters
 */
class EMA_f {
  public:
    /**
     * @brief   Create an exponential moving average filter with a pole at the
     *          given location.
     * 
     * @param   pole
     *          The pole of the filter (@f$1-\alpha@f$).  
     *          Should be a value in the range 
     *          @f$ \left[0,1\right) @f$.  
     *          Zero means no filtering, and closer to one means more filtering.
     */
    EMA_f(float pole) : alpha(1 - pole) {}

    /**
     * @brief   Filter the input: Given @f$ x[n] @f$, calculate @f$ y[n] @f$.
     *
     * @param   value
     *          The new raw input value.
     * @return  The new filtered output value.
     */
    float filter(float value) {
        filtered += (value - filtered) * alpha;
        return filtered;
    }

    /// @copydoc    filter(float)
    float operator()(float value) { return filter(value); }

  private:
    float alpha;
    float filtered = 0;
};

END_AH_NAMESPACE
