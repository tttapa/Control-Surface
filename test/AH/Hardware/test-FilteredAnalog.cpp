#include <gmock/gmock.h>

#include <AH/Hardware/FilteredAnalog.hpp>

USING_AH_NAMESPACE;

using ::testing::InSequence;
using ::testing::Return;

TEST(FilteredAnalog, Hysteresis) {
    pin_t pin = A0;
    FilteredAnalog<9, 0> analog = pin;

    EXPECT_CALL(ArduinoMock::getInstance(), analogReadResolution(10));
    FilteredAnalog<9, 0>::setupADC();
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1023));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1022));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1021));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1020));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 510);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 510. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1021));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 510);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 510. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1022));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 510);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 510. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1023));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(FilteredAnalog, resetToCurrentValue) {
    pin_t pin = A0;
    FilteredAnalog<9, 2> analog = pin;

    EXPECT_CALL(ArduinoMock::getInstance(), analogReadResolution(10));
    FilteredAnalog<>::setupADC();
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(2 * 193));
    analog.resetToCurrentValue();
    EXPECT_EQ(analog.getValue(), 193);
    
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(FilteredAnalog, reset) {
    pin_t pin = A0;
    FilteredAnalog<9, 2> analog = pin;

    EXPECT_CALL(ArduinoMock::getInstance(), analogReadResolution(10));
    FilteredAnalog<>::setupADC();
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    analog.reset(503);
    EXPECT_EQ(analog.getValue(), 503);
    
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(FilteredAnalog, map) {
    pin_t pin = A0;
    FilteredAnalog<9, 0> analog = pin;
    analog.map([](analog_t x) -> analog_t { return x - 65536 / 2; });

    EXPECT_CALL(ArduinoMock::getInstance(), analogReadResolution(10));
    FilteredAnalog<9, 0>::setupADC();
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1023));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 511 - 256);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), (511 - 256.) / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1022));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 511 - 256);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), (511 - 256.) / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1021));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 511 - 256);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), (511 - 256.) / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1020));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 510 - 256);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), (510 - 256.) / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1021));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 510 - 256);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), (510 - 256.) / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1022));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 510 - 256);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), (510 - 256.) / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1023));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 511 - 256);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), (511 - 256.) / 511);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(FilteredAnalog, invert) {
    pin_t pin = A0;
    FilteredAnalog<9, 0> analog = pin;
    analog.invert();

    EXPECT_CALL(ArduinoMock::getInstance(), analogReadResolution(10));
    FilteredAnalog<9, 0>::setupADC();
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(0));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(2));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(3));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 510);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 510. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(2));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 510);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 510. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1));
    EXPECT_FALSE(analog.update());
    EXPECT_EQ(analog.getValue(), 510);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 510. / 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(0));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);
    EXPECT_FLOAT_EQ(analog.getFloatValue(), 511. / 511);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(GenericFilteredAnalog, MappingFunction) {
    struct MockMappingFunction {
        MOCK_METHOD(bool, enabled, (), (const));
        MOCK_METHOD(analog_t, map, (analog_t), (const));
        explicit operator bool() const { return enabled(); }
        analog_t operator()(analog_t x) const { return map(x); };
    };

    pin_t pin = A0;
    MockMappingFunction map;
    GenericFilteredAnalog<const MockMappingFunction &, 9, 0> analog = {
        pin,
        map,
    };

    EXPECT_CALL(ArduinoMock::getInstance(), analogReadResolution(10));
    FilteredAnalog<9, 0>::setupADC();
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1023));
    EXPECT_CALL(map, enabled()).WillOnce(Return(false));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 511);

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1023));
    EXPECT_CALL(map, enabled()).WillOnce(Return(true));
    EXPECT_CALL(map, map(65535)).WillOnce(Return(32767));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 255);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(GenericFilteredAnalog, MappingFunctionNoBool) {
    struct MockMappingFunction {
        MOCK_METHOD(analog_t, map, (analog_t), (const));
        analog_t operator()(analog_t x) const { return map(x); };
    };

    pin_t pin = A0;
    MockMappingFunction map;
    GenericFilteredAnalog<const MockMappingFunction &, 9, 0> analog = {
        pin,
        map,
    };

    EXPECT_CALL(ArduinoMock::getInstance(), analogReadResolution(10));
    FilteredAnalog<9, 0>::setupADC();
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(pin))
        .WillOnce(Return(1023));
    EXPECT_CALL(map, map(65535)).WillOnce(Return(32767));
    EXPECT_TRUE(analog.update());
    EXPECT_EQ(analog.getValue(), 255);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(GenericFilteredAnalog, NonCopyableMovableMappingFunction) {
    struct MappingFunction {
        MappingFunction() = default;
        MappingFunction(const MappingFunction &) = delete;
        MappingFunction &operator=(const MappingFunction &) = delete;
        MappingFunction(MappingFunction &&) = default;
        MappingFunction &operator=(MappingFunction &&) = default;
    };

    pin_t pin = A0;
    MappingFunction map1;
    GenericFilteredAnalog<MappingFunction, 9, 0> analog = {
        pin,
        std::move(map1),
    };

    MappingFunction map2;
    analog.map(std::move(map2));
}

TEST(GenericFilteredAnalog, NonCopyableNonMovableMappingFunction) {
    struct MappingFunction {
        MappingFunction() = default;
        MappingFunction(const MappingFunction &) = delete;
        MappingFunction &operator=(const MappingFunction &) = delete;
        MappingFunction(MappingFunction &&) = delete;
        MappingFunction &operator=(MappingFunction &&) = delete;
    };

    pin_t pin = A0;
    MappingFunction map1;
    GenericFilteredAnalog<MappingFunction &, 9, 0> analog = {
        pin,
        map1,
    };
    (void)analog;
}