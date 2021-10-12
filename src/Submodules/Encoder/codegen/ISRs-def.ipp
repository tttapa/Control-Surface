// ========================================================================== //
//                                                                            //
//                        AUTOMATICALLY GENERATED FILE                        //
//                                DO NOT EDIT!                                //
//                                                                            //
// ========================================================================== //

// Edit and re-run  ISRs.py instead

namespace EncoderISRs {

using ISR_fun_t = void (*)(void);

static ISR_fun_t getISR(uint8_t interrupt) {
  switch (interrupt) {
    #if 0 < ENCODER_ARGLIST_SIZE
    case 0: return +[] { Encoder::update(Encoder::interruptArgs[0]); };
    #endif
    #if 1 < ENCODER_ARGLIST_SIZE
    case 1: return +[] { Encoder::update(Encoder::interruptArgs[1]); };
    #endif
    #if 2 < ENCODER_ARGLIST_SIZE
    case 2: return +[] { Encoder::update(Encoder::interruptArgs[2]); };
    #endif
    #if 3 < ENCODER_ARGLIST_SIZE
    case 3: return +[] { Encoder::update(Encoder::interruptArgs[3]); };
    #endif
    #if 4 < ENCODER_ARGLIST_SIZE
    case 4: return +[] { Encoder::update(Encoder::interruptArgs[4]); };
    #endif
    #if 5 < ENCODER_ARGLIST_SIZE
    case 5: return +[] { Encoder::update(Encoder::interruptArgs[5]); };
    #endif
    #if 6 < ENCODER_ARGLIST_SIZE
    case 6: return +[] { Encoder::update(Encoder::interruptArgs[6]); };
    #endif
    #if 7 < ENCODER_ARGLIST_SIZE
    case 7: return +[] { Encoder::update(Encoder::interruptArgs[7]); };
    #endif
    #if 8 < ENCODER_ARGLIST_SIZE
    case 8: return +[] { Encoder::update(Encoder::interruptArgs[8]); };
    #endif
    #if 9 < ENCODER_ARGLIST_SIZE
    case 9: return +[] { Encoder::update(Encoder::interruptArgs[9]); };
    #endif
    #if 10 < ENCODER_ARGLIST_SIZE
    case 10: return +[] { Encoder::update(Encoder::interruptArgs[10]); };
    #endif
    #if 11 < ENCODER_ARGLIST_SIZE
    case 11: return +[] { Encoder::update(Encoder::interruptArgs[11]); };
    #endif
    #if 12 < ENCODER_ARGLIST_SIZE
    case 12: return +[] { Encoder::update(Encoder::interruptArgs[12]); };
    #endif
    #if 13 < ENCODER_ARGLIST_SIZE
    case 13: return +[] { Encoder::update(Encoder::interruptArgs[13]); };
    #endif
    #if 14 < ENCODER_ARGLIST_SIZE
    case 14: return +[] { Encoder::update(Encoder::interruptArgs[14]); };
    #endif
    #if 15 < ENCODER_ARGLIST_SIZE
    case 15: return +[] { Encoder::update(Encoder::interruptArgs[15]); };
    #endif
    #if 16 < ENCODER_ARGLIST_SIZE
    case 16: return +[] { Encoder::update(Encoder::interruptArgs[16]); };
    #endif
    #if 17 < ENCODER_ARGLIST_SIZE
    case 17: return +[] { Encoder::update(Encoder::interruptArgs[17]); };
    #endif
    #if 18 < ENCODER_ARGLIST_SIZE
    case 18: return +[] { Encoder::update(Encoder::interruptArgs[18]); };
    #endif
    #if 19 < ENCODER_ARGLIST_SIZE
    case 19: return +[] { Encoder::update(Encoder::interruptArgs[19]); };
    #endif
    #if 20 < ENCODER_ARGLIST_SIZE
    case 20: return +[] { Encoder::update(Encoder::interruptArgs[20]); };
    #endif
    #if 21 < ENCODER_ARGLIST_SIZE
    case 21: return +[] { Encoder::update(Encoder::interruptArgs[21]); };
    #endif
    #if 22 < ENCODER_ARGLIST_SIZE
    case 22: return +[] { Encoder::update(Encoder::interruptArgs[22]); };
    #endif
    #if 23 < ENCODER_ARGLIST_SIZE
    case 23: return +[] { Encoder::update(Encoder::interruptArgs[23]); };
    #endif
    #if 24 < ENCODER_ARGLIST_SIZE
    case 24: return +[] { Encoder::update(Encoder::interruptArgs[24]); };
    #endif
    #if 25 < ENCODER_ARGLIST_SIZE
    case 25: return +[] { Encoder::update(Encoder::interruptArgs[25]); };
    #endif
    #if 26 < ENCODER_ARGLIST_SIZE
    case 26: return +[] { Encoder::update(Encoder::interruptArgs[26]); };
    #endif
    #if 27 < ENCODER_ARGLIST_SIZE
    case 27: return +[] { Encoder::update(Encoder::interruptArgs[27]); };
    #endif
    #if 28 < ENCODER_ARGLIST_SIZE
    case 28: return +[] { Encoder::update(Encoder::interruptArgs[28]); };
    #endif
    #if 29 < ENCODER_ARGLIST_SIZE
    case 29: return +[] { Encoder::update(Encoder::interruptArgs[29]); };
    #endif
    #if 30 < ENCODER_ARGLIST_SIZE
    case 30: return +[] { Encoder::update(Encoder::interruptArgs[30]); };
    #endif
    #if 31 < ENCODER_ARGLIST_SIZE
    case 31: return +[] { Encoder::update(Encoder::interruptArgs[31]); };
    #endif
    #if 32 < ENCODER_ARGLIST_SIZE
    case 32: return +[] { Encoder::update(Encoder::interruptArgs[32]); };
    #endif
    #if 33 < ENCODER_ARGLIST_SIZE
    case 33: return +[] { Encoder::update(Encoder::interruptArgs[33]); };
    #endif
    #if 34 < ENCODER_ARGLIST_SIZE
    case 34: return +[] { Encoder::update(Encoder::interruptArgs[34]); };
    #endif
    #if 35 < ENCODER_ARGLIST_SIZE
    case 35: return +[] { Encoder::update(Encoder::interruptArgs[35]); };
    #endif
    #if 36 < ENCODER_ARGLIST_SIZE
    case 36: return +[] { Encoder::update(Encoder::interruptArgs[36]); };
    #endif
    #if 37 < ENCODER_ARGLIST_SIZE
    case 37: return +[] { Encoder::update(Encoder::interruptArgs[37]); };
    #endif
    #if 38 < ENCODER_ARGLIST_SIZE
    case 38: return +[] { Encoder::update(Encoder::interruptArgs[38]); };
    #endif
    #if 39 < ENCODER_ARGLIST_SIZE
    case 39: return +[] { Encoder::update(Encoder::interruptArgs[39]); };
    #endif
    #if 40 < ENCODER_ARGLIST_SIZE
    case 40: return +[] { Encoder::update(Encoder::interruptArgs[40]); };
    #endif
    #if 41 < ENCODER_ARGLIST_SIZE
    case 41: return +[] { Encoder::update(Encoder::interruptArgs[41]); };
    #endif
    #if 42 < ENCODER_ARGLIST_SIZE
    case 42: return +[] { Encoder::update(Encoder::interruptArgs[42]); };
    #endif
    #if 43 < ENCODER_ARGLIST_SIZE
    case 43: return +[] { Encoder::update(Encoder::interruptArgs[43]); };
    #endif
    #if 44 < ENCODER_ARGLIST_SIZE
    case 44: return +[] { Encoder::update(Encoder::interruptArgs[44]); };
    #endif
    #if 45 < ENCODER_ARGLIST_SIZE
    case 45: return +[] { Encoder::update(Encoder::interruptArgs[45]); };
    #endif
    #if 46 < ENCODER_ARGLIST_SIZE
    case 46: return +[] { Encoder::update(Encoder::interruptArgs[46]); };
    #endif
    #if 47 < ENCODER_ARGLIST_SIZE
    case 47: return +[] { Encoder::update(Encoder::interruptArgs[47]); };
    #endif
    #if 48 < ENCODER_ARGLIST_SIZE
    case 48: return +[] { Encoder::update(Encoder::interruptArgs[48]); };
    #endif
    #if 49 < ENCODER_ARGLIST_SIZE
    case 49: return +[] { Encoder::update(Encoder::interruptArgs[49]); };
    #endif
    #if 50 < ENCODER_ARGLIST_SIZE
    case 50: return +[] { Encoder::update(Encoder::interruptArgs[50]); };
    #endif
    #if 51 < ENCODER_ARGLIST_SIZE
    case 51: return +[] { Encoder::update(Encoder::interruptArgs[51]); };
    #endif
    #if 52 < ENCODER_ARGLIST_SIZE
    case 52: return +[] { Encoder::update(Encoder::interruptArgs[52]); };
    #endif
    #if 53 < ENCODER_ARGLIST_SIZE
    case 53: return +[] { Encoder::update(Encoder::interruptArgs[53]); };
    #endif
    #if 54 < ENCODER_ARGLIST_SIZE
    case 54: return +[] { Encoder::update(Encoder::interruptArgs[54]); };
    #endif
    #if 55 < ENCODER_ARGLIST_SIZE
    case 55: return +[] { Encoder::update(Encoder::interruptArgs[55]); };
    #endif
    #if 56 < ENCODER_ARGLIST_SIZE
    case 56: return +[] { Encoder::update(Encoder::interruptArgs[56]); };
    #endif
    #if 57 < ENCODER_ARGLIST_SIZE
    case 57: return +[] { Encoder::update(Encoder::interruptArgs[57]); };
    #endif
    #if 58 < ENCODER_ARGLIST_SIZE
    case 58: return +[] { Encoder::update(Encoder::interruptArgs[58]); };
    #endif
    #if 59 < ENCODER_ARGLIST_SIZE
    case 59: return +[] { Encoder::update(Encoder::interruptArgs[59]); };
    #endif
    default: return nullptr;
  }
};

} // namespace EncoderISRs

