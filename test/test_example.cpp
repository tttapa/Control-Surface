#include <Arduino.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(millis, millis) {
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillOnce(Return(1))
        .WillRepeatedly(Return(2));
    EXPECT_EQ(millis(), 1);
    EXPECT_EQ(millis(), 2);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(Serial, write_byte) {
    EXPECT_CALL(Serial, write(0x12));
    Serial.write(0x12);

    Mock::VerifyAndClear(&Serial);
}

TEST(Serial, write_bytes) {
    EXPECT_CALL(Serial, write(_, _))
        .With(Args<0, 1>(ElementsAre(0x00, 0x01, 0x02)));
    const uint8_t buff[] = {0x00, 0x01, 0x02};
    Serial.write(buff, 3);

    Mock::VerifyAndClear(&Serial);
}

TEST(Serial, print_string) {
    EXPECT_CALL(Serial, write(_, _))
        .With(Args<0, 1>(ElementsAre('T', 'e', 's', 't')));
    const char *str = "Test";
    Serial.print(str);

    Mock::VerifyAndClear(&Serial);
}

TEST(Serial, println_string) {
    EXPECT_CALL(Serial, write(_, _))
        .With(Args<0, 1>(ElementsAre('T', 'e', 's', 't')));
    EXPECT_CALL(Serial, write(_, _)).With(Args<0, 1>(ElementsAre('\r', '\n')));
    const char *str = "Test";
    Serial.println(str);

    Mock::VerifyAndClear(&Serial);
}

#include <Helpers/Error.hpp>

TEST(Error, exception) {
    EXPECT_THROW(ERROR("An error occured"), ErrorException);
}

inline bool ends_with(std::string const &value, std::string const &ending) {
    if (ending.size() > value.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

TEST(Error, exceptionWhat) {
    try {
        ERROR("An error occured");
        FAIL();
    } catch (ErrorException &e) {
        std::string msg = e.what();
        cout << msg << endl;
        EXPECT_TRUE(ends_with(msg, "An error occured"));
    }
}

// TEST(Fail, fail) {
//     EXPECT_TRUE(false);
// }