#include <gmock/gmock.h>

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

// -------------------------------------------------------------------------- //

#include <Banks/Transposer.hpp>

TEST(Transposer, select) {
    Transposer<-12, +12> t(1);

    EXPECT_EQ(t.getOffset(), 0);
    t.select(0);
    EXPECT_EQ(t.getOffset(), -12);
    t.select(1);
    EXPECT_EQ(t.getOffset(), -11);
    t.select(t.N - 1);
    EXPECT_EQ(t.getOffset(), +12);
}

TEST(Transposer, select12) {
    Transposer<-1, +1> t(12);

    EXPECT_EQ(t.getOffset(), 0);
    t.select(0);
    EXPECT_EQ(t.getOffset(), -12);
    t.select(1);
    EXPECT_EQ(t.getOffset(), 0);
    t.select(2);
    EXPECT_EQ(t.getOffset(), +12);
}

TEST(Transposer, setTransposition) {
    Transposer<-12, +12> t(1);

    t.setTransposition(0);
    EXPECT_EQ(t.getOffset(), 0);
    EXPECT_EQ(t.getTransposition(), 0);
    EXPECT_EQ(t.getTranspositionSemitones(), 0);
    t.setTransposition(-12);
    EXPECT_EQ(t.getOffset(), -12);
    EXPECT_EQ(t.getTransposition(), -12);
    EXPECT_EQ(t.getTranspositionSemitones(), -12);
    t.setTransposition(-11);
    EXPECT_EQ(t.getOffset(), -11);
    EXPECT_EQ(t.getTransposition(), -11);
    EXPECT_EQ(t.getTranspositionSemitones(), -11);
    t.setTransposition(+1);
    EXPECT_EQ(t.getOffset(), +1);
    EXPECT_EQ(t.getTransposition(), +1);
    EXPECT_EQ(t.getTranspositionSemitones(), +1);
    t.setTransposition(+12);
    EXPECT_EQ(t.getOffset(), +12);
    EXPECT_EQ(t.getTransposition(), +12);
    EXPECT_EQ(t.getTranspositionSemitones(), +12);
}

TEST(Transposer, setTransposition12) {
    Transposer<-1, +1> t(12);

    t.setTransposition(0);
    EXPECT_EQ(t.getOffset(), 0);
    EXPECT_EQ(t.getTransposition(), 0);
    EXPECT_EQ(t.getTranspositionSemitones(), 0);
    t.setTransposition(-1);
    EXPECT_EQ(t.getOffset(), -12);
    EXPECT_EQ(t.getTransposition(), -1);
    EXPECT_EQ(t.getTranspositionSemitones(), -12);
    t.setTransposition(+1);
    EXPECT_EQ(t.getOffset(), +12);
    EXPECT_EQ(t.getTransposition(), +1);
    EXPECT_EQ(t.getTranspositionSemitones(), +12);
}