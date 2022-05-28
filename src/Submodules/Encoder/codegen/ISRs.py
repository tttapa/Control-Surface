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

// Edit and re-run {basename(__file__)} instead

namespace EncoderISRs {{

using ISR_fun_t = void (*)();

"""

footer = f"""
}} // namespace EncoderISRs

"""

ISRs = """[[maybe_unused]] static ISR_fun_t getISR(int interrupt) {
  switch (interrupt) {
"""
for i in range(max_num_interrupts):
    ISRs += f'    #if {i} < CS_ENCODER_ARGLIST_SIZE\n'
    ISRs += f'    case {i}: return +[]() CS_ENCODER_ISR_ATTR {{ AHEncoder::interruptArgs[{i}]->update(); }};\n'
    ISRs += f'    #endif\n'
ISRs += f"""    default: return nullptr;
    static_assert(CS_ENCODER_ARGLIST_SIZE <= {max_num_interrupts},
                  "Error: more external interrupts than expected. "
                  "Increase {basename(__file__)}:max_num_interrupts and try again.");
  }}
}};
"""

with open(join(dirname(__file__), 'ISRs-def.ipp'), 'w') as f:
    f.write(header)
    f.write(ISRs)
    f.write(footer)
