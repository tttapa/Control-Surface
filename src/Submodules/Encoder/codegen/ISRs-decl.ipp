// ========================================================================== //
//                                                                            //
//                        AUTOMATICALLY GENERATED FILE                        //
//                                DO NOT EDIT!                                //
//                                                                            //
// ========================================================================== //

// Edit and re-run  ISRs.py instead

#if defined(ENCODER_USE_INTERRUPTS) && !defined(ENCODER_OPTIMIZE_INTERRUPTS)

namespace EncoderISRs {

using ISR_fun_t = void (*)(void);

static ISR_fun_t getISR(uint8_t interrupt);

} // namespace EncoderISRs

#endif
