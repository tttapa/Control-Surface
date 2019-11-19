/**
 * @boards  AVR, AVR USB
 */

#include <Arduino_Helpers.h>

#include <AH/STL/algorithm>
#include <AH/STL/array>
#include <AH/STL/cmath>
#include <AH/STL/complex>
#include <AH/STL/cstdint>
#include <AH/STL/cstdlib>
#include <AH/STL/initializer_list>
#include <AH/STL/limits>
#include <AH/STL/new>
#include <AH/STL/numeric>
#include <AH/STL/optional>
#include <AH/STL/type_traits>
#include <AH/STL/utility>

using std::is_same;

#define TEST_CMATH_FUNCTION(Fun)                                               \
  static_assert(is_same<decltype(Fun((float)1)), float>::value,                \
                "cmath error " #Fun "(float)");                                \
  static_assert(is_same<decltype(Fun((double)1)), double>::value,              \
                "cmath error " #Fun "(double)");                               \
  static_assert(is_same<decltype(Fun((long double)1)), long double>::value,    \
                "cmath error " #Fun "(long double)");

#define TEST_CMATH_FUNCTION_I(Fun)                                             \
  TEST_CMATH_FUNCTION(Fun);                                                    \
  static_assert(is_same<decltype(Fun((int)1)), int>::value,                    \
                "cmath error " #Fun "(int)");

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
    "cmath error " #Fun "(long double)");

#define TEST_CMATH_FUNCTION2(Fun)                                              \
  static_assert(is_same<decltype(Fun((float)1, (float)1)), float>::value,      \
                "cmath error " #Fun "(float)");                                \
  static_assert(is_same<decltype(Fun((double)1, (double)1)), double>::value,   \
                "cmath error " #Fun "(double)");                               \
  static_assert(is_same<decltype(Fun((long double)1, (long double)1)),         \
                        long double>::value,                                   \
                "cmath error " #Fun "(long double)");

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

TEST_CMATH_FUNCTION(std::asinh);
TEST_CMATH_FUNCTION(std::exp2);

TEST_CMATH_FUNCTION_I(std::abs);

TEST_CMATH_FUNCTION2(std::hypot);
TEST_CMATH_FUNCTION3(std::fma);

void setup() {
#ifdef __cpp_lib_optional
  std::optional<int> opt = std::nullopt;
  opt = 1;
  Serial.println(*opt);
#endif
}
void loop() {}