#include <Arduino.h>
#include <gtest/gtest.h>

class MyEnv : public ::testing::Environment {
  public:
    void SetUp() override { ArduinoMock::begin(); }
    void TearDown() override { ArduinoMock::end(); }
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new MyEnv);
    return RUN_ALL_TESTS();
}

const char *__asan_default_options() {
    return "detect_invalid_pointer_pairs=2";
}