#if __GNUC__ >= 5
// Disable GCC 5's -Wsuggest-override warnings in gtest
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wsuggest-override"
#endif
#include <gtest/gtest.h>
#if __GNUC__ >= 5
# pragma GCC diagnostic pop
#endif