/* ✔ */

#pragma once

#include <stdint.h>

/**
 * @brief   A class for single-pole infinite impulse response filters
 *          or exponential moving average filters.
 *
 * Straightforward implementation of the difference equation with a slight
 * optimization by using a factor of two as the pole location (this means
 * that no division or floating point operations are required).
 *
 * Difference equation: @f$ y[n] = \alpha·x[n]+(1-\alpha)·y[n-1] @f$
 * where @f$ \alpha = \left(\frac{1}{2}\right)^{K} @f$, @f$ x @f$ is the
 * input sequence, and @f$ y @f$ is the output sequence.
 *
 * [An in-depth explanation of the EMA filter]
 * (https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/)
 *
 * @tparam  K
 *          The amount of bits to shift by. This determines the location
 *          of the pole in the EMA transfer function, and therefore the
 *          cut-off frequency.
 * @tparam  int_t
 *          The (signed) integer type to use for the input, intermediate values
 *          and the output.
 *          Should be at least @f$ M+K @f$ bits wide, where @f$ M @f$
 *          is the maximum number of bits of the input.
 *          In case of the Arduino's built-in ADC,
 *          @f$ M = 10 = \log_2(1024) @f$.
 *
 */
template <uint8_t K, class int_t>
class EMA {
  public:
    /**
     * @brief   Filter the input: Given @f$ x[n] @f$, calculate @f$ y[n] @f$.
     *
     * @param   input
     *          The new raw input value.
     * @return  The new filtered output value.
     */
    int_t filter(int_t input) {
        filtered += input;
        int_t output = (filtered + fixedPointAHalf) >> K;
        filtered -= output;
        return output;
    }

  private:
    int_t filtered = 0;
    constexpr static int_t fixedPointAHalf = 1 << (K - 1);
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
 */
class EMA_f {
  public:
    /**
     * @brief   Create an exponential moving average filter with a pole at the
     *          given location.
     * 
     * @param   pole
     *          The pole of the filter (@f$1-\alpha@f$).
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
        filtered = filtered + (value - filtered) * alpha;
        return filtered;
    }

    /**
     * @brief   Filter the input: Given @f$ x[n] @f$, calculate @f$ y[n] @f$.
     *
     * @param   value
     *          The new raw input value.
     * @return  The new filtered output value.
     */
    float operator()(float value) { return filter(value); }

  private:
    const float alpha;
    float filtered = 0;
};