#ifndef Encoder_h_
#define Encoder_h_

#include "gmock/gmock.h"

class Encoder {
  public:
    Encoder(int, int) {}
    MOCK_CONST_METHOD0(read, long());
};

#endif