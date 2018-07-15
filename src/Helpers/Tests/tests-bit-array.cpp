// g++ -Wall -std=c++11 -o tests -I/home/pieter/lib/Catch2/single_include *.cpp && ./tests

#if !defined(ARDUINO) && !defined(GOOGLETEST)

#include <catch.hpp>

#include "../BitArray.hpp"

TEST_CASE("Construct", "") {
    BitArray ba(16);
    for (int i = 0; i < 16; i++)
        REQUIRE(ba.get(i) == false);
}

TEST_CASE("Set & Get & Clear & Get", "") {
    BitArray ba(16);
    ba.set(12);
    ba.set(7);
    for (int i = 0; i < 16; i++)
        if (i == 12 || i == 7)
            REQUIRE(ba.get(i) == true);
        else
            REQUIRE(ba.get(i) == false);
    ba.clear(12);
    for (int i = 0; i < 16; i++)
        if (i == 7)
            REQUIRE(ba.get(i) == true);
        else
            REQUIRE(ba.get(i) == false);
    ba.set(7, false);
    ba.set(6, true);
    for (int i = 0; i < 16; i++)
        if (i == 6)
            REQUIRE(ba.get(i) == true);
        else
            REQUIRE(ba.get(i) == false);
}

#endif // ifndef ARDUINO