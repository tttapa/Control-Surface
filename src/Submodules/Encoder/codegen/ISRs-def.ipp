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
    #if 0 < CS_ENCODER_ARGLIST_SIZE
    case 0: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[0]); };
    #endif
    #if 1 < CS_ENCODER_ARGLIST_SIZE
    case 1: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[1]); };
    #endif
    #if 2 < CS_ENCODER_ARGLIST_SIZE
    case 2: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[2]); };
    #endif
    #if 3 < CS_ENCODER_ARGLIST_SIZE
    case 3: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[3]); };
    #endif
    #if 4 < CS_ENCODER_ARGLIST_SIZE
    case 4: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[4]); };
    #endif
    #if 5 < CS_ENCODER_ARGLIST_SIZE
    case 5: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[5]); };
    #endif
    #if 6 < CS_ENCODER_ARGLIST_SIZE
    case 6: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[6]); };
    #endif
    #if 7 < CS_ENCODER_ARGLIST_SIZE
    case 7: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[7]); };
    #endif
    #if 8 < CS_ENCODER_ARGLIST_SIZE
    case 8: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[8]); };
    #endif
    #if 9 < CS_ENCODER_ARGLIST_SIZE
    case 9: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[9]); };
    #endif
    #if 10 < CS_ENCODER_ARGLIST_SIZE
    case 10: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[10]); };
    #endif
    #if 11 < CS_ENCODER_ARGLIST_SIZE
    case 11: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[11]); };
    #endif
    #if 12 < CS_ENCODER_ARGLIST_SIZE
    case 12: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[12]); };
    #endif
    #if 13 < CS_ENCODER_ARGLIST_SIZE
    case 13: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[13]); };
    #endif
    #if 14 < CS_ENCODER_ARGLIST_SIZE
    case 14: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[14]); };
    #endif
    #if 15 < CS_ENCODER_ARGLIST_SIZE
    case 15: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[15]); };
    #endif
    #if 16 < CS_ENCODER_ARGLIST_SIZE
    case 16: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[16]); };
    #endif
    #if 17 < CS_ENCODER_ARGLIST_SIZE
    case 17: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[17]); };
    #endif
    #if 18 < CS_ENCODER_ARGLIST_SIZE
    case 18: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[18]); };
    #endif
    #if 19 < CS_ENCODER_ARGLIST_SIZE
    case 19: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[19]); };
    #endif
    #if 20 < CS_ENCODER_ARGLIST_SIZE
    case 20: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[20]); };
    #endif
    #if 21 < CS_ENCODER_ARGLIST_SIZE
    case 21: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[21]); };
    #endif
    #if 22 < CS_ENCODER_ARGLIST_SIZE
    case 22: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[22]); };
    #endif
    #if 23 < CS_ENCODER_ARGLIST_SIZE
    case 23: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[23]); };
    #endif
    #if 24 < CS_ENCODER_ARGLIST_SIZE
    case 24: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[24]); };
    #endif
    #if 25 < CS_ENCODER_ARGLIST_SIZE
    case 25: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[25]); };
    #endif
    #if 26 < CS_ENCODER_ARGLIST_SIZE
    case 26: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[26]); };
    #endif
    #if 27 < CS_ENCODER_ARGLIST_SIZE
    case 27: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[27]); };
    #endif
    #if 28 < CS_ENCODER_ARGLIST_SIZE
    case 28: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[28]); };
    #endif
    #if 29 < CS_ENCODER_ARGLIST_SIZE
    case 29: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[29]); };
    #endif
    #if 30 < CS_ENCODER_ARGLIST_SIZE
    case 30: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[30]); };
    #endif
    #if 31 < CS_ENCODER_ARGLIST_SIZE
    case 31: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[31]); };
    #endif
    #if 32 < CS_ENCODER_ARGLIST_SIZE
    case 32: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[32]); };
    #endif
    #if 33 < CS_ENCODER_ARGLIST_SIZE
    case 33: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[33]); };
    #endif
    #if 34 < CS_ENCODER_ARGLIST_SIZE
    case 34: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[34]); };
    #endif
    #if 35 < CS_ENCODER_ARGLIST_SIZE
    case 35: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[35]); };
    #endif
    #if 36 < CS_ENCODER_ARGLIST_SIZE
    case 36: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[36]); };
    #endif
    #if 37 < CS_ENCODER_ARGLIST_SIZE
    case 37: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[37]); };
    #endif
    #if 38 < CS_ENCODER_ARGLIST_SIZE
    case 38: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[38]); };
    #endif
    #if 39 < CS_ENCODER_ARGLIST_SIZE
    case 39: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[39]); };
    #endif
    #if 40 < CS_ENCODER_ARGLIST_SIZE
    case 40: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[40]); };
    #endif
    #if 41 < CS_ENCODER_ARGLIST_SIZE
    case 41: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[41]); };
    #endif
    #if 42 < CS_ENCODER_ARGLIST_SIZE
    case 42: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[42]); };
    #endif
    #if 43 < CS_ENCODER_ARGLIST_SIZE
    case 43: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[43]); };
    #endif
    #if 44 < CS_ENCODER_ARGLIST_SIZE
    case 44: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[44]); };
    #endif
    #if 45 < CS_ENCODER_ARGLIST_SIZE
    case 45: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[45]); };
    #endif
    #if 46 < CS_ENCODER_ARGLIST_SIZE
    case 46: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[46]); };
    #endif
    #if 47 < CS_ENCODER_ARGLIST_SIZE
    case 47: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[47]); };
    #endif
    #if 48 < CS_ENCODER_ARGLIST_SIZE
    case 48: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[48]); };
    #endif
    #if 49 < CS_ENCODER_ARGLIST_SIZE
    case 49: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[49]); };
    #endif
    #if 50 < CS_ENCODER_ARGLIST_SIZE
    case 50: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[50]); };
    #endif
    #if 51 < CS_ENCODER_ARGLIST_SIZE
    case 51: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[51]); };
    #endif
    #if 52 < CS_ENCODER_ARGLIST_SIZE
    case 52: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[52]); };
    #endif
    #if 53 < CS_ENCODER_ARGLIST_SIZE
    case 53: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[53]); };
    #endif
    #if 54 < CS_ENCODER_ARGLIST_SIZE
    case 54: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[54]); };
    #endif
    #if 55 < CS_ENCODER_ARGLIST_SIZE
    case 55: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[55]); };
    #endif
    #if 56 < CS_ENCODER_ARGLIST_SIZE
    case 56: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[56]); };
    #endif
    #if 57 < CS_ENCODER_ARGLIST_SIZE
    case 57: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[57]); };
    #endif
    #if 58 < CS_ENCODER_ARGLIST_SIZE
    case 58: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[58]); };
    #endif
    #if 59 < CS_ENCODER_ARGLIST_SIZE
    case 59: return +[]() CS_ENCODER_ISR_ATTR { Encoder::update(Encoder::interruptArgs[59]); };
    #endif
    default: return nullptr;
  }
};

} // namespace EncoderISRs
