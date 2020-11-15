#!/usr/bin/env python3

from os.path import dirname, join, basename

max_num_interrupts = 60

header = f"""\
// ========================================================================== //
//                                                                            //
//                        AUTOMATICALLY GENERATED FILE                        //
//                                DO NOT EDIT!                                //
//                                                                            //
// ========================================================================== //

// Edit and re-run  {basename(__file__)} instead

namespace EncoderISRs {{

using ISR_fun_t = void (*)(void);

"""

footer = f"""
}} // namespace EncoderISRs

"""

ISRs = """static ISR_fun_t getISR(uint8_t interrupt) {
  switch (interrupt) {
"""
for i in range(max_num_interrupts):
    ISRs += f'    #if {i} < ENCODER_ARGLIST_SIZE\n'
    ISRs += f'    case {i}: return +[] {{ Encoder::update(Encoder::interruptArgs[{i}]); }};\n'
    ISRs += f'    #endif\n'
ISRs += """    default: return nullptr;
  }
};
"""

with open(join(dirname(__file__), 'ISRs-def.ipp'), 'w') as f:
    f.write(header)
    f.write(ISRs)
    f.write(footer)
