#pragma once

#include "ExtendedIOElement.hpp"
#include <Helpers/ArrayHelpers.hpp>

template <pin_t N>
class StaticSizeExtendedIOElement : public ExtendedIOElement {
  protected:
    StaticSizeExtendedIOElement() : ExtendedIOElement{N} {}

  public:
    Array<pin_t, N> pins() const {
        return generateIncrementalArray<pin_t, N>(getStart());
    }
};