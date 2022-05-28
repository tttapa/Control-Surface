#pragma once

#include <gmock/gmock.h>

class EncoderMock {
  public:
    MOCK_CONST_METHOD0(read, long());
};

class AHEncoder {
  public:
    AHEncoder(int, int) : mock(nullptr) {}
    AHEncoder(const EncoderMock &mock) : mock(&mock) {}
    AHEncoder(EncoderMock &&) = delete;
    AHEncoder(const AHEncoder &) = default;
    long read() const { return mock ? mock->read() : 0; }

  private:
    const EncoderMock *mock;
};
