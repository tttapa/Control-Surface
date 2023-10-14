#include <AH/STL/algorithm>
#include <AH/STL/array>
#include <AH/STL/bitset>
#include <AH/STL/climits>
#include <AH/STL/cmath>
#include <AH/STL/complex>
#include <AH/STL/cstdint>
#include <AH/STL/cstdlib>
#include <AH/STL/initializer_list>
#include <AH/STL/limits>
#include <AH/STL/memory>
#include <AH/STL/new>
#include <AH/STL/numeric>
#include <AH/STL/optional>
#include <AH/STL/tuple>
#include <AH/STL/type_traits>
#include <AH/STL/utility>
#include <AH/STL/vector>

#include <AH/Arduino-Wrapper.h>
#include <AH/PrintStream/PrintStream.hpp>

AH_DIAGNOSTIC_WERROR() // ESP32 -Werror=c++0x-compat fix

long double f() {
  return rand();
}

using std::is_same;
using AH::operator<<;
using AH::endl;

// clang-format off
#define TEST_CMATH_FUNCTION(Fun)                                               \
  static_assert(is_same<decltype(Fun((float)1)), float>::value,                \
                "cmath error " #Fun "(float)");                                \
  static_assert(is_same<decltype(Fun((double)1)), double>::value,              \
                "cmath error " #Fun "(double)");                               \
  static_assert(is_same<decltype(Fun((long double)1)), long double>::value,    \
                "cmath error " #Fun "(long double)");                          \
  Serial.println(Fun((float)f()));                                             \
  Serial.println(Fun((double)f()));                                            \
  Serial.println((double)Fun((long double)f()));

#define TEST_CMATH_FUNCTION_I(Fun)                                             \
  TEST_CMATH_FUNCTION(Fun);                                                    \
  static_assert(is_same<decltype(Fun((int)1)), int>::value,                    \
                "cmath error " #Fun "(int)");                                  \
  Serial.println(Fun((int)f()));

#define TEST_CMATH_FUNCTION3(Fun)                                              \
  static_assert(                                                               \
    is_same<decltype(Fun((float)1, (float)1, (float)1)), float>::value,        \
    "cmath error " #Fun "(float)");                                            \
  static_assert(                                                               \
    is_same<decltype(Fun((double)1, (double)1, (double)1)), double>::value,    \
    "cmath error " #Fun "(double)");                                           \
  static_assert(                                                               \
    is_same<decltype(Fun((long double)1, (long double)1, (long double)1)),     \
            long double>::value,                                               \
    "cmath error " #Fun "(long double)");                                      \
  Serial.println(Fun((float)f(), (float)f(), (float)f()));                     \
  Serial.println(Fun((double)f(), (double)f(), (double)f()));                  \
  Serial.println(                                                              \
    (double)Fun((long double)f(), (long double)f(), (long double)f()));

#define TEST_CMATH_FUNCTION2(Fun)                                              \
  static_assert(is_same<decltype(Fun((float)1, (float)1)), float>::value,      \
                "cmath error " #Fun "(float)");                                \
  static_assert(is_same<decltype(Fun((double)1, (double)1)), double>::value,   \
                "cmath error " #Fun "(double)");                               \
  static_assert(is_same<decltype(Fun((long double)1, (long double)1)),         \
                        long double>::value,                                   \
                "cmath error " #Fun "(long double)");                          \
  Serial.println(Fun((float)f(), (float)f()));                                 \
  Serial.println(Fun((double)f(), (double)f()));                               \
  Serial.println((double)Fun((long double)f(), (long double)f()));
// clang-format on

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
#ifdef __cpp_lib_optional
  std::optional<int> opt = std::nullopt;
  opt = 1;
  Serial.println(*opt);
#endif
  std::bitset<128> bs;
  bs[0] = 1;
  bs[2] = 1;
  Serial << bs << endl;
  auto ptr = AH::make_unique<std::array<int, 10>>();
  ptr->operator[](2) = 42;
  Serial.println(ptr->at(2));

  TEST_CMATH_FUNCTION(std::round);
  TEST_CMATH_FUNCTION(std::sin);
  TEST_CMATH_FUNCTION(std::cos);
  TEST_CMATH_FUNCTION(std::tan);
  TEST_CMATH_FUNCTION(std::asin);
  TEST_CMATH_FUNCTION(std::acos);
  TEST_CMATH_FUNCTION(std::atan);
  TEST_CMATH_FUNCTION(std::exp);
  TEST_CMATH_FUNCTION(std::log);
  TEST_CMATH_FUNCTION(std::log10);

#ifndef __AVR__
  TEST_CMATH_FUNCTION(std::asinh);
  TEST_CMATH_FUNCTION(std::exp2);
#endif

  TEST_CMATH_FUNCTION_I(std::abs);

  TEST_CMATH_FUNCTION2(std::hypot);
  TEST_CMATH_FUNCTION3(std::fma);

  std::vector<int> vec {1, 2, 3, 4};
  vec.reserve(100);
  vec.resize(50);
  vec[1] = -2;
  for (int i : vec)
    Serial.println(i);
}

void loop() {}

AH_DIAGNOSTIC_POP()