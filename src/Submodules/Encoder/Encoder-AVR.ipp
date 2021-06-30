inline void Encoder::update(Encoder_internal_state_t *arg) {
    // The compiler believes this is just 1 line of code, so
    // it will inline this function into each interrupt
    // handler.  That's a tiny bit faster, but grows the code.
    // Especially when used with ENCODER_OPTIMIZE_INTERRUPTS,
    // the inline nature allows the ISR prologue and epilogue
    // to only save/restore necessary registers, for very nice
    // speed increase.
    asm volatile("ld        r30, X+"
                 "\n\t"
                 "ld        r31, X+"
                 "\n\t"
                 "ld        r24, Z"
                 "\n\t" // r24 = pin1 input
                 "ld        r30, X+"
                 "\n\t"
                 "ld        r31, X+"
                 "\n\t"
                 "ld        r25, Z"
                 "\n\t" // r25 = pin2 input
                 "ld        r30, X+"
                 "\n\t" // r30 = pin1 mask
                 "ld        r31, X+"
                 "\n\t" // r31 = pin2 mask
                 "ld        r22, X"
                 "\n\t" // r22 = state
                 "andi        r22, 3"
                 "\n\t"
                 "and        r24, r30"
                 "\n\t"
                 "breq        L%=1"
                 "\n\t" // if (pin1)
                 "ori        r22, 4"
                 "\n\t" //        state |= 4
                 "L%=1:"
                 "and        r25, r31"
                 "\n\t"
                 "breq        L%=2"
                 "\n\t" // if (pin2)
                 "ori        r22, 8"
                 "\n\t" //        state |= 8
                 "L%=2:"
                 "ldi        r30, lo8(pm(L%=table))"
                 "\n\t"
                 "ldi        r31, hi8(pm(L%=table))"
                 "\n\t"
                 "add        r30, r22"
                 "\n\t"
                 "adc        r31, __zero_reg__"
                 "\n\t"
                 "asr        r22"
                 "\n\t"
                 "asr        r22"
                 "\n\t"
                 "st        X+, r22"
                 "\n\t" // store new state
                 "ld        r22, X+"
                 "\n\t"
                 "ld        r23, X+"
                 "\n\t"
                 "ld        r24, X+"
                 "\n\t"
                 "ld        r25, X+"
                 "\n\t"
                 "ijmp"
                 "\n\t" // jumps to update_finishup()
                 // TODO move this table to another static function,
                 // so it doesn't get needlessly duplicated.  Easier
                 // said than done, due to linker issues and inlining
                 "L%=table:"
                 "\n\t"
                 "rjmp        L%=end"
                 "\n\t" // 0
                 "rjmp        L%=plus1"
                 "\n\t" // 1
                 "rjmp        L%=minus1"
                 "\n\t" // 2
                 "rjmp        L%=plus2"
                 "\n\t" // 3
                 "rjmp        L%=minus1"
                 "\n\t" // 4
                 "rjmp        L%=end"
                 "\n\t" // 5
                 "rjmp        L%=minus2"
                 "\n\t" // 6
                 "rjmp        L%=plus1"
                 "\n\t" // 7
                 "rjmp        L%=plus1"
                 "\n\t" // 8
                 "rjmp        L%=minus2"
                 "\n\t" // 9
                 "rjmp        L%=end"
                 "\n\t" // 10
                 "rjmp        L%=minus1"
                 "\n\t" // 11
                 "rjmp        L%=plus2"
                 "\n\t" // 12
                 "rjmp        L%=minus1"
                 "\n\t" // 13
                 "rjmp        L%=plus1"
                 "\n\t" // 14
                 "rjmp        L%=end"
                 "\n\t" // 15
                 "L%=minus2:"
                 "\n\t"
                 "subi        r22, 2"
                 "\n\t"
                 "sbci        r23, 0"
                 "\n\t"
                 "sbci        r24, 0"
                 "\n\t"
                 "sbci        r25, 0"
                 "\n\t"
                 "rjmp        L%=store"
                 "\n\t"
                 "L%=minus1:"
                 "\n\t"
                 "subi        r22, 1"
                 "\n\t"
                 "sbci        r23, 0"
                 "\n\t"
                 "sbci        r24, 0"
                 "\n\t"
                 "sbci        r25, 0"
                 "\n\t"
                 "rjmp        L%=store"
                 "\n\t"
                 "L%=plus2:"
                 "\n\t"
                 "subi        r22, 254"
                 "\n\t"
                 "rjmp        L%=z"
                 "\n\t"
                 "L%=plus1:"
                 "\n\t"
                 "subi        r22, 255"
                 "\n\t"
                 "L%=z:"
                 "sbci        r23, 255"
                 "\n\t"
                 "sbci        r24, 255"
                 "\n\t"
                 "sbci        r25, 255"
                 "\n\t"
                 "L%=store:"
                 "\n\t"
                 "st        -X, r25"
                 "\n\t"
                 "st        -X, r24"
                 "\n\t"
                 "st        -X, r23"
                 "\n\t"
                 "st        -X, r22"
                 "\n\t"
                 "L%=end:"
                 "\n"
                 :
                 : "x"(arg)
                 : "r22", "r23", "r24", "r25", "r30", "r31");
}
