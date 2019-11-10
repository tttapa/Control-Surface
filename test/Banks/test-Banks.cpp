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

W_SUGGEST_OVERRIDE_OFF

template <uint8_t N>
class TestInputBankable : public BankableMIDIInput<N> {
  public:
    TestInputBankable(Bank<N> &bank, BankType type)
        : BankableMIDIInput<N>(bank, type) {}

    MOCK_METHOD0(onBankSettingChange, void());
};

W_SUGGEST_OVERRIDE_ON

TEST(Bank, onBankSettingChange) {
    Bank<10> bank = {4};
    TestInputBankable<10> ib1 = {bank, CHANGE_ADDRESS};
    EXPECT_CALL(ib1, onBankSettingChange());
    bank.select(6);
    {
        TestInputBankable<10> ib2 = {bank, CHANGE_ADDRESS};
        EXPECT_CALL(ib1, onBankSettingChange());
        EXPECT_CALL(ib2, onBankSettingChange());
        bank.select(5);
    }
    TestInputBankable<10> ib3 = {bank, CHANGE_ADDRESS};
    // Check that destructor correctly removed ib2 from bank
    EXPECT_CALL(ib1, onBankSettingChange());
    EXPECT_CALL(ib3, onBankSettingChange());
    bank.select(4);
}