/**
 * Simple Quaternion test.
 *
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 *
 * Written by PieterP, 2020-03-24  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>

#include <AH/Math/Quaternion.hpp>
#include <AH/PrintStream/PrintStream.hpp>

bool close(float actual, float expected, float atol) {
    return std::abs(actual - expected) <= atol;
}

void setup() {
    Serial.begin(115200);

    Vec3f vector {-0.414578098794425, 0.829156197588850, 0.375000000000000};
    Quaternion result   = Quaternion::fromDirection(vector);
    Quaternion expected {0.829156197588850, -0.5, -0.25, 0.0};
    float atol = 1e-7;

    Serial << setprecision(7) << "Result = " << result << endl;

    Serial << boolalpha;
    Serial << close(result.w, expected.w, atol) << endl;
    Serial << close(result.x, expected.x, atol) << endl;
    Serial << close(result.y, expected.y, atol) << endl;
    Serial << close(result.z, expected.z, atol) << endl;

    // Make sure it satisfies the definition of `fromDirection()`.
    Vec3f diff = result.rotate({0, 0, 1}) - vector.normalized();
    Serial << close(diff.norm(), 0, atol) << endl;
}

void loop() {}