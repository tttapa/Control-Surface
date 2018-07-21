#ifndef Encoder_h_
#define Encoder_h_

#include "gmock/gmock.h"

class EncoderMock {
  public:
    MOCK_CONST_METHOD0(read, long());
};

class Encoder {
  public:
    Encoder(const EncoderMock &mock) : mock(mock) {}
    Encoder(const Encoder &) = default;
    long read() const { return mock.read(); }

  private:
    const EncoderMock &mock;
};

#endif