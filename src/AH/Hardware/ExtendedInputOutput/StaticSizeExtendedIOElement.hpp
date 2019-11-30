/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "ExtendedIOElement.hpp"
#include <AH/Containers/ArrayHelpers.hpp>

BEGIN_AH_NAMESPACE

/** 
 * @brief   A class for ExtendedIOElement%s with a fixed size.
 * 
 * This class is to make it easier to get an array of all pins of the element.
 */
template <uint16_t N>
class StaticSizeExtendedIOElement : public ExtendedIOElement { // LCOV_EXCL_LINE
  protected:
    StaticSizeExtendedIOElement() : ExtendedIOElement{N} {} // LCOV_EXCL_LINE

  public:
    /**
     * @brief   Get an array containing all pins of the element.
     */
    Array<pin_t, N> pins() const {
        return generateIncrementalArray<pin_t, N>(getStart());
    }

    static constexpr uint16_t length() { return N; }
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
