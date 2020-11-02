#include <gmock-wrapper.h>

#include <Banks/Bank.hpp>

USING_CS_NAMESPACE;
using AH::ErrorException;

TEST(OutputBank, OutputBank) {
    OutputBank ob = {4};
    EXPECT_EQ(ob.getSelection(), 0);
    EXPECT_EQ(ob.getTracksPerBank(), 4);
}

TEST(OutputBank, initialSetting) {
    OutputBank ob = {4, 1};
    EXPECT_EQ(ob.getSelection(), 1);
    EXPECT_EQ(ob.getTracksPerBank(), 4);
}

TEST(OutputBank, noTracks) {
    try {
        OutputBank ob = {0};
        (void)ob;
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x4573);
    }
}

TEST(OutputBank, select) {
    OutputBank ob = {4};
    ob.select(99);
    EXPECT_EQ(ob.getSelection(), 99);
}

// -------------------------------------------------------------------------- //

TEST(Bank, Bank) {
    Bank<10> bank = {4, 1};
    EXPECT_EQ(bank.getInitialSelection(), 1);
    EXPECT_EQ(bank.getSelection(), 1);
    EXPECT_EQ(bank.getTracksPerBank(), 4);
}

TEST(Bank, select) {
    Bank<10> bank = {4};
    bank.select(5);
    EXPECT_EQ(bank.getSelection(), 5);
}

TEST(Bank, selectOutOfBounds) {
    Bank<10> bank = {4};
    bank.select(9);
    try {
        bank.select(10);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0xFFFE);
    }
}
