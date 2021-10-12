strings = [
    ('REC_RDY_1', 0x00),  # In Out
    ('REC_RDY_2', 0x01),  # In Out
    ('REC_RDY_3', 0x02),  # In Out
    ('REC_RDY_4', 0x03),  # In Out
    ('REC_RDY_5', 0x04),  # In Out
    ('REC_RDY_6', 0x05),  # In Out
    ('REC_RDY_7', 0x06),  # In Out
    ('REC_RDY_8', 0x07),  # In Out
    ('SOLO_1', 0x08),  # In Out
    ('SOLO_2', 0x09),  # In Out
    ('SOLO_3', 0x0A),  # In Out
    ('SOLO_4', 0x0B),  # In Out
    ('SOLO_5', 0x0C),  # In Out
    ('SOLO_6', 0x0D),  # In Out
    ('SOLO_7', 0x0E),  # In Out
    ('SOLO_8', 0x0F),  # In Out
    ('MUTE_1', 0x10),  # In Out
    ('MUTE_2', 0x11),  # In Out
    ('MUTE_3', 0x12),  # In Out
    ('MUTE_4', 0x13),  # In Out
    ('MUTE_5', 0x14),  # In Out
    ('MUTE_6', 0x15),  # In Out
    ('MUTE_7', 0x16),  # In Out
    ('MUTE_8', 0x17),  # In Out
    ('SELECT_1', 0x18),  # In Out
    ('SELECT_2', 0x19),  # In Out
    ('SELECT_3', 0x1A),  # In Out
    ('SELECT_4', 0x1B),  # In Out
    ('SELECT_5', 0x1C),  # In Out
    ('SELECT_6', 0x1D),  # In Out
    ('SELECT_7', 0x1E),  # In Out
    ('SELECT_8', 0x1F),  # In Out
    ('V_POT_SELECT_1', 0x20),  # In Out
    ('V_POT_SELECT_2', 0x21),  # In Out
    ('V_POT_SELECT_3', 0x22),  # In Out
    ('V_POT_SELECT_4', 0x23),  # In Out
    ('V_POT_SELECT_5', 0x24),  # In Out
    ('V_POT_SELECT_6', 0x25),  # In Out
    ('V_POT_SELECT_7', 0x26),  # In Out
    ('V_POT_SELECT_8', 0x27),  # In Out
    ('ASSIGN_TRACK', 0x28),  # In Out
    ('ASSIGN_SEND', 0x29),  # In Out
    ('ASSIGN_PAN', 0x2A),  # In Out
    ('ASSIGN_PLUGIN', 0x2B),  # In Out
    ('ASSIGN_EQ', 0x2C),  # In Out
    ('ASSIGN_INSTR', 0x2D),  # In Out
    ('BANK_LEFT', 0x2E),  #    Out
    ('BANK_RIGHT', 0x2F),  #    Out
    ('CHANNEL_LEFT', 0x30),  #    Out
    ('CHANNEL_RIGHT', 0x31),  #    Out
    ('FLIP', 0x32),  # In Out
    ('GLOBAL_VIEW', 0x33),  # In Out
    ('NAME_VALUE', 0x34),  #    Out
    ('SMPTE_BEATS', 0x35),  #    Out
    ('F1', 0x36),  #    Out
    ('F2', 0x37),  #    Out
    ('F3', 0x38),  #    Out
    ('F4', 0x39),  #    Out
    ('F5', 0x3A),  #    Out
    ('F6', 0x3B),  #    Out
    ('F7', 0x3C),  #    Out
    ('F8', 0x3D),  #    Out
    ('VIEW_MIDI', 0x3E),  #    Out
    ('VIEW_INPUTS', 0x3F),  #    Out
    ('VIEW_AUDIO', 0x40),  #    Out
    ('VIEW_INSTR', 0x41),  #    Out
    ('VIEW_AUX', 0x42),  #    Out
    ('VIEW_BUSSES', 0x43),  #    Out
    ('VIEW_OUTPUTS', 0x44),  #    Out
    ('VIEW_USER', 0x45),  #    Out
    ('SHIFT', 0x46),  #    Out
    ('OPTION', 0x47),  #    Out
    ('CONTROL', 0x48),  #    Out
    ('CMD_ALT', 0x49),  #    Out
    ('AUTOMATION_READ_OFF', 0x4A),  # In Out
    ('AUTOMATION_WRITE', 0x4B),  # In Out
    ('AUTOMATION_TRIM', 0x4C),  # In Out
    ('AUTOMATION_TOUCH', 0x4D),  # In Out
    ('AUTOMATION_LATCH', 0x4E),  # In Out
    ('GROUP', 0x4F),  # In Out
    ('SAVE', 0x50),  # In Out
    ('UNDO', 0x51),  # In Out
    ('CANCEL', 0x52),  #    Out
    ('ENTER', 0x53),  #    Out
    ('MARKER', 0x54),  # In Out
    ('NUDGE', 0x55),  # In Out
    ('CYCLE', 0x56),  # In Out
    ('DROP', 0x57),  # In Out
    ('REPLACE', 0x58),  # In Out
    ('CLICK', 0x59),  # In Out
    ('SOLO', 0x5A),  # In Out
    ('REWIND', 0x5B),  # In Out
    ('FAST_FWD', 0x5C),  # In Out
    ('STOP', 0x5D),  # In Out
    ('PLAY', 0x5E),  # In Out
    ('RECORD', 0x5F),  # In Out
    ('UP', 0x60),  #    Out
    ('DOWN', 0x61),  #    Out
    ('LEFT', 0x62),  #    Out
    ('RIGHT', 0x63),  #    Out
    ('ZOOM', 0x64),  # In Out
    ('SCRUB', 0x65),  # In Out
    ('USER_SWITCH_A', 0x66),  #    Out
    ('USER_SWITCH_B', 0x67),  #    Out
    ('FADER_TOUCH_1', 0x68),  #    Out
    ('FADER_TOUCH_2', 0x69),  #    Out
    ('FADER_TOUCH_3', 0x6A),  #    Out
    ('FADER_TOUCH_4', 0x6B),  #    Out
    ('FADER_TOUCH_5', 0x6C),  #    Out
    ('FADER_TOUCH_6', 0x6D),  #    Out
    ('FADER_TOUCH_7', 0x6E),  #    Out
    ('FADER_TOUCH_8', 0x6F),  #    Out
    ('FADER_TOUCH_MASTER', 0x70),  #    Out
    ('SMPTE', 0x71),  # In
    ('BEATS', 0x72),  # In
    ('RUDE_SOLO', 0x73),  # In
    ('0x74', 0x74),  #
    ('0x75', 0x75),  #
    ('RELAY', 0x76),  # In
]

lutname = 'MCU_Note_Name_LUT'
functionname = 'getMCUNameFromNoteNumber'
filename = 'MCUNameFromNoteNumber'

################################################################################

import os
import sys

pythonfilename = os.path.basename(__file__)

warning = """
// =============== AUTOMATICALLY GENERATED FILE: DO NOT EDIT! =============== //
//                                                                            //
// Edit {filename} instead. {sp} //
""".format(filename=pythonfilename, sp=' ' * (59 - len(pythonfilename)))

hpp = warning + """
#pragma once

#include <AH/Arduino-Wrapper.h>  // FlashString_t
#include <Settings/NamespaceSettings.hpp>

"""

cpp = warning + """
#include "{filename}.hpp"

""".format(filename=filename)

ctr = 0
for kv in strings:
    assert (ctr == kv[1])
    ctr += 1
    cpp += 'const static char flashstr_0x{:02X}[] PROGMEM = '.format(kv[1])
    cpp += '"' + kv[0] + '";'
    cpp += '\n'
cpp += '\n'
cpp += 'static FlashString_t const ' + lutname + '[] PROGMEM = {'
cpp += '\n'
for kv in strings:
    cpp += '    reinterpret_cast<FlashString_t>'
    cpp += '(flashstr_0x{:02X}), '.format(kv[1])
    cpp += '\n'

cpp += '};'
cpp += '\n'

hpp += """\
BEGIN_CS_NAMESPACE

namespace MCU {{

/// @addtogroup    MCU_Notes
/// @{{

/**
 * @brief   Convert a note number to the name of the corresponding Mackie 
 * Control Universal function.
 *
 * @param   note 
 *          The note number of the function to look up.
 */
FlashString_t {functionname}(uint8_t note);

/// @}}

}} // namespace MCU 

END_CS_NAMESPACE
""".format(functionname=functionname)

cpp += """
BEGIN_CS_NAMESPACE
namespace MCU {{

FlashString_t {functionname}(uint8_t note) {{
    const static char invalid[] PROGMEM = "<out-of-bounds>";
    if (note >= 0x{length:02X})
        return reinterpret_cast<FlashString_t>(invalid);
    const void *flashptr = pgm_read_ptr_near({lutname} + note);
    return reinterpret_cast<FlashString_t>(flashptr);
}}

}} // namespace MCU 
END_CS_NAMESPACE
""".format(functionname=functionname, lutname=lutname, length=len(strings))

with open(os.path.join(sys.path[0], filename + '.hpp'), 'w') as header, \
     open(os.path.join(sys.path[0], filename + '.cpp'), 'w') as source:
    header.write(hpp)
    source.write(cpp)