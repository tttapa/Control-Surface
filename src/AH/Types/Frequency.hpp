#pragma once

#include <AH/Settings/NamespaceSettings.hpp>

BEGIN_AH_NAMESPACE

/// Type-safe class for frequency values.
class Frequency {
  public:
    explicit constexpr Frequency(unsigned long hertz) : hertz(hertz) {}
    constexpr operator unsigned long() const { return hertz; }

  private:
    unsigned long hertz;
};
constexpr Frequency operator"" _Hz(unsigned long long hz) {
    return Frequency{(unsigned long)hz};
}
constexpr Frequency operator"" _kHz(long double khz) {
    return Frequency{(unsigned long)(khz * 1E3l)};
}
constexpr Frequency operator"" _kHz(unsigned long long khz) {
    return Frequency{(unsigned long)(khz * 1E3)};
}
constexpr Frequency operator"" _MHz(long double mhz) {
    return Frequency{(unsigned long)(mhz * 1E6l)};
}
constexpr Frequency operator"" _MHz(unsigned long long mhz) {
    return Frequency{(unsigned long)(mhz * 1E6)};
}

END_AH_NAMESPACE
