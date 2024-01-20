#include <MIDI_Interfaces/BLEMIDI/BLEAPI.hpp>
#include <MIDI_Interfaces/BLEMIDI/BLERingBuf.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

USING_CS_NAMESPACE;

TEST(BLERingBuf, popempty) {
    BLERingBuf<1024> buf;
    BLEDataView data;
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
}

template <size_t N>
BLEDataView view(const char (&str)[N]) {
    return BLEDataView {reinterpret_cast<const uint8_t *>(str), N};
}

TEST(BLERingBuf, pushpop) {
    BLERingBuf<1024> buf;
    EXPECT_TRUE(buf.push(view("abc")));
    EXPECT_TRUE(buf.push(view("defgh")));
    BLEDataView data;
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    ASSERT_EQ(data.length, 4);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "abc");
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    ASSERT_EQ(data.length, 6);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "defgh");
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
}

TEST(BLERingBuf, pushpopOdd) {
    BLERingBuf<1024> buf;
    EXPECT_TRUE(buf.push(view("ab")));
    EXPECT_TRUE(buf.push(view("cdef")));
    BLEDataView data;
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    ASSERT_EQ(data.length, 3);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "ab");
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    ASSERT_EQ(data.length, 5);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "cdef");
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
}

TEST(BLERingBuf, pushfull) {
    BLERingBuf<12> buf;
    // |HH|--|--|--|--|--|
    EXPECT_TRUE(buf.push(view("abc")));
    // |HH|HH|ab|c-|--|--|
    EXPECT_FALSE(buf.push(view("def")));
    BLEDataView data;
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    // |--|HH|ab|c-|--|--|
    ASSERT_EQ(data.length, 4);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "abc");
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    // |--|--|--|HH|--|--|
}

TEST(BLERingBuf, pushpopNonContiguous) {
    BLERingBuf<12> buf;
    // |HH|--|--|--|--|--|
    EXPECT_TRUE(buf.push(view("abc")));
    // |HH|HH|ab|c-|--|--|
    BLEDataView data;
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    // |--|HH|ab|c-|--|--|
    ASSERT_EQ(data.length, 4);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "abc");
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    // |--|--|--|HH|--|--|
    EXPECT_TRUE(buf.push(view("d\0efg")));
    // |HH|ef|g-|HH|HH|d-|
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    // |HH|ef|g-|--|HH|d-|
    ASSERT_EQ(data.length, 2);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "d");
    EXPECT_EQ(buf.pop(data), BLEDataType::Continuation);
    // |HH|ef|g-|--|--|--|
    ASSERT_EQ(data.length, 4);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "efg");
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    // |--|--|HH|--|--|--|
    EXPECT_TRUE(buf.push(view("hij\0k")));
    // |HH|k-|HH|HH|hi|j-|
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    // |HH|k-|--|HH|hi|j-|
    ASSERT_EQ(data.length, 4);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "hij");
    EXPECT_EQ(buf.pop(data), BLEDataType::Continuation);
    // |HH|g-|--|--|--|--|
    ASSERT_EQ(data.length, 2);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "k");
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    // |--|HH|--|--|--|--|
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    EXPECT_TRUE(buf.push(view("lmnop")));
    // |--|HH|HH|lm|no|p-|
    EXPECT_TRUE(buf.push({}));
    // |HH|HH|HH|lm|no|p-|
    EXPECT_FALSE(buf.push({}));
    // |HH|HH|HH|lm|no|p-|
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    // |HH|--|HH|lm|no|p-|
    ASSERT_EQ(data.length, 6);
    EXPECT_STREQ(reinterpret_cast<const char *>(data.data), "lmnop");
    EXPECT_EQ(buf.pop(data), BLEDataType::Packet);
    // |HH|--|--|--|--|HH|
    ASSERT_EQ(data.length, 0);
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    // |HH|--|--|--|--|--|
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
    EXPECT_EQ(buf.pop(data), BLEDataType::None);
}
