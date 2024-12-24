#include <AH/PrintStream/PrintStream.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
USING_AH_NAMESPACE;

TEST(Debug, HexDump) {
    std::stringstream s;
    uint8_t d[] = {0x11, 0x23, 0xF7, 0xFF, 0x00};
    s << HexDump(d, sizeof(d));
    EXPECT_EQ(s.str(), "11 23 F7 FF 00");
}
