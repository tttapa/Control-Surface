#pragma once

#include <Def/Channel.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/// @addtogroup    MIDIConstants
/// @{

/**
 * @defgroup    MCU_Constants   Mackie Control Universal constants
 * @brief   Names and note and controller numbers for the Mackie Control 
 *          Universal (MCU) protocol.
 * @{
 */

/// @defgroup MCU_Notes MCU MIDI Note controls
/// @{

constexpr uint8_t REC_RDY_1            = 0x00; ///< Record/Ready 1      (In/Out)
constexpr uint8_t REC_RDY_2            = 0x01; ///< Record/Ready 2      (In/Out)
constexpr uint8_t REC_RDY_3            = 0x02; ///< Record/Ready 3      (In/Out)
constexpr uint8_t REC_RDY_4            = 0x03; ///< Record/Ready 4      (In/Out)
constexpr uint8_t REC_RDY_5            = 0x04; ///< Record/Ready 5      (In/Out)
constexpr uint8_t REC_RDY_6            = 0x05; ///< Record/Ready 6      (In/Out)
constexpr uint8_t REC_RDY_7            = 0x06; ///< Record/Ready 7      (In/Out)
constexpr uint8_t REC_RDY_8            = 0x07; ///< Record/Ready 8      (In/Out)

constexpr uint8_t SOLO_1               = 0x08; ///< Solo 1              (In/Out)
constexpr uint8_t SOLO_2               = 0x09; ///< Solo 2              (In/Out)
constexpr uint8_t SOLO_3               = 0x0A; ///< Solo 3              (In/Out)
constexpr uint8_t SOLO_4               = 0x0B; ///< Solo 4              (In/Out)
constexpr uint8_t SOLO_5               = 0x0C; ///< Solo 5              (In/Out)
constexpr uint8_t SOLO_6               = 0x0D; ///< Solo 6              (In/Out)
constexpr uint8_t SOLO_7               = 0x0E; ///< Solo 7              (In/Out)
constexpr uint8_t SOLO_8               = 0x0F; ///< Solo 8              (In/Out)

constexpr uint8_t MUTE_1               = 0x10; ///< Mute 1              (In/Out)
constexpr uint8_t MUTE_2               = 0x11; ///< Mute 2              (In/Out)
constexpr uint8_t MUTE_3               = 0x12; ///< Mute 3              (In/Out)
constexpr uint8_t MUTE_4               = 0x13; ///< Mute 4              (In/Out)
constexpr uint8_t MUTE_5               = 0x14; ///< Mute 5              (In/Out)
constexpr uint8_t MUTE_6               = 0x15; ///< Mute 6              (In/Out)
constexpr uint8_t MUTE_7               = 0x16; ///< Mute 7              (In/Out)
constexpr uint8_t MUTE_8               = 0x17; ///< Mute 8              (In/Out)

constexpr uint8_t SELECT_1             = 0x18; ///< Select 1            (In/Out)
constexpr uint8_t SELECT_2             = 0x19; ///< Select 2            (In/Out)
constexpr uint8_t SELECT_3             = 0x1A; ///< Select 3            (In/Out)
constexpr uint8_t SELECT_4             = 0x1B; ///< Select 4            (In/Out)
constexpr uint8_t SELECT_5             = 0x1C; ///< Select 5            (In/Out)
constexpr uint8_t SELECT_6             = 0x1D; ///< Select 6            (In/Out)
constexpr uint8_t SELECT_7             = 0x1E; ///< Select 7            (In/Out)
constexpr uint8_t SELECT_8             = 0x1F; ///< Select 8            (In/Out)

constexpr uint8_t V_POT_SELECT_1       = 0x20; ///< V-Pot select 1      (In/Out)
constexpr uint8_t V_POT_SELECT_2       = 0x21; ///< V-Pot select 2      (In/Out)
constexpr uint8_t V_POT_SELECT_3       = 0x22; ///< V-Pot select 3      (In/Out)
constexpr uint8_t V_POT_SELECT_4       = 0x23; ///< V-Pot select 4      (In/Out)
constexpr uint8_t V_POT_SELECT_5       = 0x24; ///< V-Pot select 5      (In/Out)
constexpr uint8_t V_POT_SELECT_6       = 0x25; ///< V-Pot select 6      (In/Out)
constexpr uint8_t V_POT_SELECT_7       = 0x26; ///< V-Pot select 7      (In/Out)
constexpr uint8_t V_POT_SELECT_8       = 0x27; ///< V-Pot select 8      (In/Out)

constexpr uint8_t ASSIGN_TRACK         = 0x28; ///< Assign Track        (In/Out)
constexpr uint8_t ASSIGN_SEND          = 0x29; ///< Assign Send         (In/Out)
constexpr uint8_t ASSIGN_PAN           = 0x2A; ///< Assign Pan          (In/Out)
constexpr uint8_t ASSIGN_PLUGIN        = 0x2B; ///< Assign Pluggin      (In/Out)
constexpr uint8_t ASSIGN_EQ            = 0x2C; ///< Assign Equalizer    (In/Out)
constexpr uint8_t ASSIGN_INSTR         = 0x2D; ///< Assign Instrument   (In/Out)

constexpr uint8_t BANK_LEFT            = 0x2E; ///< Banks: Bank Left       (Out)
constexpr uint8_t BANK_RIGHT           = 0x2F; ///< Banks: Bank Right      (Out)
constexpr uint8_t CHANNEL_LEFT         = 0x30; ///< Banks: Channel Left    (Out)
constexpr uint8_t CHANNEL_RIGHT        = 0x31; ///< Banks: Channel Right   (Out)

constexpr uint8_t FLIP                 = 0x32; ///< Flip                (In/Out)
constexpr uint8_t GLOBAL_VIEW          = 0x33; ///< Global View         (In/Out)
constexpr uint8_t NAME_VALUE           = 0x34; ///< Name/Value             (Out)
constexpr uint8_t SMPTE_BEATS          = 0x35; ///< SMPTE/Beats            (Out)

constexpr uint8_t F1                   = 0x36; ///< Function key 1         (Out)
constexpr uint8_t F2                   = 0x37; ///< Function key 2         (Out)
constexpr uint8_t F3                   = 0x38; ///< Function key 3         (Out)
constexpr uint8_t F4                   = 0x39; ///< Function key 4         (Out)
constexpr uint8_t F5                   = 0x3A; ///< Function key 5         (Out)
constexpr uint8_t F6                   = 0x3B; ///< Function key 6         (Out)
constexpr uint8_t F7                   = 0x3C; ///< Function key 7         (Out)
constexpr uint8_t F8                   = 0x3D; ///< Function key 8         (Out)

constexpr uint8_t VIEW_MIDI            = 0x3E; ///< View MIDI tracks       (Out)
constexpr uint8_t VIEW_INPUTS          = 0x3F; ///< View inputs            (Out)
constexpr uint8_t VIEW_AUDIO           = 0x40; ///< View audio tracks      (Out)
constexpr uint8_t VIEW_INSTR           = 0x41; ///< View audio instrument  (Out)
constexpr uint8_t VIEW_AUX             = 0x42; ///< View auxiliary         (Out)
constexpr uint8_t VIEW_BUSSES          = 0x43; ///< View busses            (Out)
constexpr uint8_t VIEW_OUTPUTS         = 0x44; ///< View outputs           (Out)
constexpr uint8_t VIEW_USER            = 0x45; ///< View user              (Out)

constexpr uint8_t SHIFT                = 0x46; ///< Shift                  (Out)
constexpr uint8_t OPTION               = 0x47; ///< Option                 (Out)
constexpr uint8_t CONTROL              = 0x48; ///< Control                (Out)
constexpr uint8_t CMD_ALT              = 0x49; ///< Command/Alt            (Out)

constexpr uint8_t AUTOMATION_READ_OFF  = 0x4A; ///< Automation read/off (In/Out)
constexpr uint8_t AUTOMATION_WRITE     = 0x4B; ///< Automation write    (In/Out)
constexpr uint8_t AUTOMATION_TRIM      = 0x4C; ///< Automation trim     (In/Out)
constexpr uint8_t AUTOMATION_TOUCH     = 0x4D; ///< Automation touch    (In/Out)
constexpr uint8_t AUTOMATION_LATCH     = 0x4E; ///< Automation latch    (In/Out)

constexpr uint8_t GROUP                = 0x4F; ///< Group               (In/Out)
constexpr uint8_t SAVE                 = 0x50; ///< Save                (In/Out)
constexpr uint8_t UNDO                 = 0x51; ///< Undo                (In/Out)
constexpr uint8_t CANCEL               = 0x52; ///< Cancel                 (Out)
constexpr uint8_t ENTER                = 0x53; ///< Enter                  (Out)

constexpr uint8_t MARKER               = 0x54; ///< Marker              (In/Out)
constexpr uint8_t NUDGE                = 0x55; ///< Nudge               (In/Out)
constexpr uint8_t CYCLE                = 0x56; ///< Cycle               (In/Out)
constexpr uint8_t DROP                 = 0x57; ///< Drop                (In/Out)
constexpr uint8_t REPLACE              = 0x58; ///< Replace             (In/Out)
constexpr uint8_t CLICK                = 0x59; ///< Click               (In/Out)
constexpr uint8_t SOLO                 = 0x5A; ///< Solo               (In/Out)

constexpr uint8_t REWIND               = 0x5B; ///< Rewind              (In/Out)
constexpr uint8_t FAST_FWD             = 0x5C; ///< Fast forward        (In/Out)
constexpr uint8_t STOP                 = 0x5D; ///< Stop                (In/Out)
constexpr uint8_t PLAY                 = 0x5E; ///< Play                (In/Out)
constexpr uint8_t RECORD               = 0x5F; ///< Record              (In/Out)

constexpr uint8_t UP                   = 0x60; ///< Cursor up              (Out)
constexpr uint8_t DOWN                 = 0x61; ///< Cursor down            (Out)
constexpr uint8_t LEFT                 = 0x62; ///< Cursor left            (Out)
constexpr uint8_t RIGHT                = 0x63; ///< Cursor right           (Out)

constexpr uint8_t ZOOM                 = 0x64; ///< Zoom                (In/Out)
constexpr uint8_t SCRUB                = 0x65; ///< Scrub               (In/Out)

constexpr uint8_t USER_SWITCH_A        = 0x66; ///< User switch A          (Out)
constexpr uint8_t USER_SWITCH_B        = 0x67; ///< User switch B          (Out)

constexpr uint8_t FADER_TOUCH_1        = 0x68; ///< Fader touch 1          (Out)
constexpr uint8_t FADER_TOUCH_2        = 0x69; ///< Fader touch 2          (Out)
constexpr uint8_t FADER_TOUCH_3        = 0x6A; ///< Fader touch 3          (Out)
constexpr uint8_t FADER_TOUCH_4        = 0x6B; ///< Fader touch 4          (Out)
constexpr uint8_t FADER_TOUCH_5        = 0x6C; ///< Fader touch 5          (Out)
constexpr uint8_t FADER_TOUCH_6        = 0x6D; ///< Fader touch 6          (Out)
constexpr uint8_t FADER_TOUCH_7        = 0x6E; ///< Fader touch 7          (Out)
constexpr uint8_t FADER_TOUCH_8        = 0x6F; ///< Fader touch 8          (Out)
constexpr uint8_t FADER_TOUCH_MASTER   = 0x70; ///< Fader touch master     (Out)

constexpr uint8_t SMPTE                = 0x71; ///< SMPTE LED               (In)
constexpr uint8_t BEATS                = 0x72; ///< Beats LED               (In)

constexpr uint8_t RUDE_SOLO            = 0x73; ///< Rude solo light         (In)

constexpr uint8_t RELAY                = 0x76; ///< Relay                   (In)

/// @}

/// @defgroup MCU_CC MCU Control Change controls
/// @{

constexpr uint8_t V_POT_1              = 0x10; ///< V-Pot 1 (Relative)     (Out)
constexpr uint8_t V_POT_2              = 0x11; ///< V-Pot 2 (Relative)     (Out)
constexpr uint8_t V_POT_3              = 0x12; ///< V-Pot 3 (Relative)     (Out)
constexpr uint8_t V_POT_4              = 0x13; ///< V-Pot 4 (Relative)     (Out)
constexpr uint8_t V_POT_5              = 0x14; ///< V-Pot 5 (Relative)     (Out)
constexpr uint8_t V_POT_6              = 0x15; ///< V-Pot 6 (Relative)     (Out)
constexpr uint8_t V_POT_7              = 0x16; ///< V-Pot 7 (Relative)     (Out)
constexpr uint8_t V_POT_8              = 0x17; ///< V-Pot 8 (Relative)     (Out)

constexpr uint8_t EXTERNAL_CONTROLLER  = 0x2E; ///< External controller    (Out)

constexpr uint8_t JOG_WHEEL            = 0x3C; ///< Jog wheel (Relative)   (Out)

/// @}

/// @defgroup MCU_PB MCU Pitch Bend controls
/// @{

constexpr Channel VOLUME_1             = CHANNEL_1; ///< Volume fader 1 (In/Out)
constexpr Channel VOLUME_2             = CHANNEL_2; ///< Volume fader 2 (In/Out)
constexpr Channel VOLUME_3             = CHANNEL_3; ///< Volume fader 3 (In/Out)
constexpr Channel VOLUME_4             = CHANNEL_4; ///< Volume fader 4 (In/Out)
constexpr Channel VOLUME_5             = CHANNEL_5; ///< Volume fader 5 (In/Out)
constexpr Channel VOLUME_6             = CHANNEL_6; ///< Volume fader 6 (In/Out)
constexpr Channel VOLUME_7             = CHANNEL_7; ///< Volume fader 7 (In/Out)
constexpr Channel VOLUME_8             = CHANNEL_8; ///< Volume fader 8 (In/Out)

constexpr Channel MASTER_VOLUME        = CHANNEL_9; ///< Master volume  (In/Out)

/// @}

/// @}

/// @}

} // namespace MCU

END_CS_NAMESPACE

#include "MCUNameFromNoteNumber.hpp"