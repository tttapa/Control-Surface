namespace MCU {

const uint8_t REC_RDY                = 0x00; // In Out
const uint8_t SOLO                   = 0x08; // In Out
const uint8_t MUTE                   = 0x10; // In Out
const uint8_t SELECT                 = 0x18; // In Out
const uint8_t V_POT_SELECT           = 0x20; //    Out

const uint8_t ASSIGN_TRACK           = 0x28; // In Out
const uint8_t ASSIGN_SEND            = 0x29; // In Out
const uint8_t ASSIGN_PAN             = 0x2A; // In Out
const uint8_t ASSIGN_PLUGIN          = 0x2B; // In Out
const uint8_t ASSIGN_EQ              = 0x2C; // In Out
const uint8_t ASSIGN_INSTR           = 0x2D; // In Out

const uint8_t BANK_LEFT              = 0x2E; //    Out
const uint8_t BANK_RIGHT             = 0x2F; //    Out
const uint8_t CHANNEL_LEFT           = 0x30; //    Out
const uint8_t CHANNEL_RIGHT          = 0x31; //    Out

const uint8_t FLIP                   = 0x32; // In Out
const uint8_t GLOBAL_VIEW            = 0x33; // In Out
const uint8_t NAME_VALUE             = 0x34; //    Out
const uint8_t SMPTE_BEATS            = 0x35; //    Out

const uint8_t F1                     = 0x36; //    Out
const uint8_t F2                     = 0x37; //    Out
const uint8_t F3                     = 0x38; //    Out
const uint8_t F4                     = 0x39; //    Out
const uint8_t F5                     = 0x3A; //    Out
const uint8_t F6                     = 0x3B; //    Out
const uint8_t F7                     = 0x3C; //    Out
const uint8_t F8                     = 0x3D; //    Out

const uint8_t VIEW_MIDI              = 0x3E; //    Out
const uint8_t VIEW_INPUTS            = 0x3F; //    Out
const uint8_t VIEW_AUDIO             = 0x40; //    Out
const uint8_t VIEW_INSTR             = 0x41; //    Out
const uint8_t VIEW_AUX               = 0x42; //    Out
const uint8_t VIEW_BUSSES            = 0x43; //    Out
const uint8_t VIEW_OUTPUTS           = 0x44; //    Out
const uint8_t VIEW_USER              = 0x45; //    Out

const uint8_t SHIFT                  = 0x46; //    Out
const uint8_t OPTION                 = 0x47; //    Out
const uint8_t CONTROL                = 0x48; //    Out
const uint8_t CMD_ALT                = 0x49; //    Out

const uint8_t AUTOMATION_READ_OFF    = 0x4A; // In Out
const uint8_t AUTOMATION_WRITE       = 0x4B; // In Out
const uint8_t AUTOMATION_TRIM        = 0x4C; // In Out
const uint8_t AUTOMATION_TOUCH       = 0x4D; // In Out
const uint8_t AUTOMATION_LATCH       = 0x4E; // In Out

const uint8_t GROUP                  = 0x4F; // In Out
const uint8_t SAVE                   = 0x50; // In Out
const uint8_t UNDO                   = 0x51; // In Out
const uint8_t CANCEL                 = 0x52; //    Out
const uint8_t ENTER                  = 0x53; //    Out

const uint8_t MARKER                 = 0x54; // In Out
const uint8_t NUDGE                  = 0x55; // In Out
const uint8_t CYCLE                  = 0x56; // In Out
const uint8_t DROP                   = 0x57; // In Out
const uint8_t REPLACE                = 0x58; // In Out
const uint8_t CLICK                  = 0x59; // In Out
const uint8_t SOLO_G                 = 0x5A; // In Out

const uint8_t REWIND                 = 0x5B; // In Out
const uint8_t FAST_FWD               = 0x5C; // In Out
const uint8_t STOP                   = 0x5D; // In Out
const uint8_t PLAY                   = 0x5E; // In Out
const uint8_t RECORD                 = 0x5F; // In Out

const uint8_t UP                     = 0x60; //    Out
const uint8_t DOWN                   = 0x61; //    Out
const uint8_t LEFT                   = 0x62; //    Out
const uint8_t RIGHT                  = 0x63; //    Out

const uint8_t ZOOM                   = 0x64; // In Out
const uint8_t SCRUB                  = 0x65; // In Out

const uint8_t USER_SWITCH_A          = 0x66; //    Out
const uint8_t USER_SWITCH_B          = 0x67; //    Out

const uint8_t FADER_TOUCH            = 0x68; //    Out
const uint8_t FADER_TOUCH_MASTER     = 0x70; //    Out

const uint8_t SMPTE                  = 0x71; // In
const uint8_t BEATS                  = 0x72; // In

const uint8_t RUDE_SOLO              = 0x73; // In

const uint8_t RELAY                  = 0x76; // In

};
