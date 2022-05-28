// ========================================================================== //
//                                                                            //
//                        AUTOMATICALLY GENERATED FILE                        //
//                                DO NOT EDIT!                                //
//                                                                            //
// ========================================================================== //

// Edit and re-run ISRs.py instead

namespace EncoderISRs {

using ISR_fun_t = void (*)();

[[maybe_unused]] static ISR_fun_t getISR(int interrupt) {
  switch (interrupt) {
    #if 0 < CS_ENCODER_ARGLIST_SIZE
    case 0: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[0]->update(); };
    #endif
    #if 1 < CS_ENCODER_ARGLIST_SIZE
    case 1: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[1]->update(); };
    #endif
    #if 2 < CS_ENCODER_ARGLIST_SIZE
    case 2: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[2]->update(); };
    #endif
    #if 3 < CS_ENCODER_ARGLIST_SIZE
    case 3: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[3]->update(); };
    #endif
    #if 4 < CS_ENCODER_ARGLIST_SIZE
    case 4: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[4]->update(); };
    #endif
    #if 5 < CS_ENCODER_ARGLIST_SIZE
    case 5: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[5]->update(); };
    #endif
    #if 6 < CS_ENCODER_ARGLIST_SIZE
    case 6: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[6]->update(); };
    #endif
    #if 7 < CS_ENCODER_ARGLIST_SIZE
    case 7: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[7]->update(); };
    #endif
    #if 8 < CS_ENCODER_ARGLIST_SIZE
    case 8: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[8]->update(); };
    #endif
    #if 9 < CS_ENCODER_ARGLIST_SIZE
    case 9: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[9]->update(); };
    #endif
    #if 10 < CS_ENCODER_ARGLIST_SIZE
    case 10: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[10]->update(); };
    #endif
    #if 11 < CS_ENCODER_ARGLIST_SIZE
    case 11: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[11]->update(); };
    #endif
    #if 12 < CS_ENCODER_ARGLIST_SIZE
    case 12: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[12]->update(); };
    #endif
    #if 13 < CS_ENCODER_ARGLIST_SIZE
    case 13: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[13]->update(); };
    #endif
    #if 14 < CS_ENCODER_ARGLIST_SIZE
    case 14: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[14]->update(); };
    #endif
    #if 15 < CS_ENCODER_ARGLIST_SIZE
    case 15: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[15]->update(); };
    #endif
    #if 16 < CS_ENCODER_ARGLIST_SIZE
    case 16: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[16]->update(); };
    #endif
    #if 17 < CS_ENCODER_ARGLIST_SIZE
    case 17: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[17]->update(); };
    #endif
    #if 18 < CS_ENCODER_ARGLIST_SIZE
    case 18: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[18]->update(); };
    #endif
    #if 19 < CS_ENCODER_ARGLIST_SIZE
    case 19: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[19]->update(); };
    #endif
    #if 20 < CS_ENCODER_ARGLIST_SIZE
    case 20: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[20]->update(); };
    #endif
    #if 21 < CS_ENCODER_ARGLIST_SIZE
    case 21: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[21]->update(); };
    #endif
    #if 22 < CS_ENCODER_ARGLIST_SIZE
    case 22: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[22]->update(); };
    #endif
    #if 23 < CS_ENCODER_ARGLIST_SIZE
    case 23: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[23]->update(); };
    #endif
    #if 24 < CS_ENCODER_ARGLIST_SIZE
    case 24: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[24]->update(); };
    #endif
    #if 25 < CS_ENCODER_ARGLIST_SIZE
    case 25: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[25]->update(); };
    #endif
    #if 26 < CS_ENCODER_ARGLIST_SIZE
    case 26: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[26]->update(); };
    #endif
    #if 27 < CS_ENCODER_ARGLIST_SIZE
    case 27: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[27]->update(); };
    #endif
    #if 28 < CS_ENCODER_ARGLIST_SIZE
    case 28: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[28]->update(); };
    #endif
    #if 29 < CS_ENCODER_ARGLIST_SIZE
    case 29: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[29]->update(); };
    #endif
    #if 30 < CS_ENCODER_ARGLIST_SIZE
    case 30: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[30]->update(); };
    #endif
    #if 31 < CS_ENCODER_ARGLIST_SIZE
    case 31: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[31]->update(); };
    #endif
    #if 32 < CS_ENCODER_ARGLIST_SIZE
    case 32: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[32]->update(); };
    #endif
    #if 33 < CS_ENCODER_ARGLIST_SIZE
    case 33: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[33]->update(); };
    #endif
    #if 34 < CS_ENCODER_ARGLIST_SIZE
    case 34: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[34]->update(); };
    #endif
    #if 35 < CS_ENCODER_ARGLIST_SIZE
    case 35: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[35]->update(); };
    #endif
    #if 36 < CS_ENCODER_ARGLIST_SIZE
    case 36: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[36]->update(); };
    #endif
    #if 37 < CS_ENCODER_ARGLIST_SIZE
    case 37: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[37]->update(); };
    #endif
    #if 38 < CS_ENCODER_ARGLIST_SIZE
    case 38: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[38]->update(); };
    #endif
    #if 39 < CS_ENCODER_ARGLIST_SIZE
    case 39: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[39]->update(); };
    #endif
    #if 40 < CS_ENCODER_ARGLIST_SIZE
    case 40: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[40]->update(); };
    #endif
    #if 41 < CS_ENCODER_ARGLIST_SIZE
    case 41: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[41]->update(); };
    #endif
    #if 42 < CS_ENCODER_ARGLIST_SIZE
    case 42: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[42]->update(); };
    #endif
    #if 43 < CS_ENCODER_ARGLIST_SIZE
    case 43: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[43]->update(); };
    #endif
    #if 44 < CS_ENCODER_ARGLIST_SIZE
    case 44: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[44]->update(); };
    #endif
    #if 45 < CS_ENCODER_ARGLIST_SIZE
    case 45: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[45]->update(); };
    #endif
    #if 46 < CS_ENCODER_ARGLIST_SIZE
    case 46: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[46]->update(); };
    #endif
    #if 47 < CS_ENCODER_ARGLIST_SIZE
    case 47: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[47]->update(); };
    #endif
    #if 48 < CS_ENCODER_ARGLIST_SIZE
    case 48: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[48]->update(); };
    #endif
    #if 49 < CS_ENCODER_ARGLIST_SIZE
    case 49: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[49]->update(); };
    #endif
    #if 50 < CS_ENCODER_ARGLIST_SIZE
    case 50: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[50]->update(); };
    #endif
    #if 51 < CS_ENCODER_ARGLIST_SIZE
    case 51: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[51]->update(); };
    #endif
    #if 52 < CS_ENCODER_ARGLIST_SIZE
    case 52: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[52]->update(); };
    #endif
    #if 53 < CS_ENCODER_ARGLIST_SIZE
    case 53: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[53]->update(); };
    #endif
    #if 54 < CS_ENCODER_ARGLIST_SIZE
    case 54: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[54]->update(); };
    #endif
    #if 55 < CS_ENCODER_ARGLIST_SIZE
    case 55: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[55]->update(); };
    #endif
    #if 56 < CS_ENCODER_ARGLIST_SIZE
    case 56: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[56]->update(); };
    #endif
    #if 57 < CS_ENCODER_ARGLIST_SIZE
    case 57: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[57]->update(); };
    #endif
    #if 58 < CS_ENCODER_ARGLIST_SIZE
    case 58: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[58]->update(); };
    #endif
    #if 59 < CS_ENCODER_ARGLIST_SIZE
    case 59: return +[]() CS_ENCODER_ISR_ATTR { AHEncoder::interruptArgs[59]->update(); };
    #endif
    default: return nullptr;
    static_assert(CS_ENCODER_ARGLIST_SIZE <= 60,
                  "Error: more external interrupts than expected. "
                  "Increase ISRs.py:max_num_interrupts and try again.");
  }
};

} // namespace EncoderISRs
