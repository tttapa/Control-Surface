#include <gtest/gtest.h>

#include <Helpers/EMA.h>

TEST(EMA, overflow) {
    EMA<2, int16_t> ema;

// TODO
/*
    int16_t out = ema.filter(16);
    out = ema.filter(0);
    out = ema.filter(16);
*/
}