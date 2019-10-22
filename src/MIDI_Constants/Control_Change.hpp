#pragma once

#include <stdint.h>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// @addtogroup    MIDIConstants
/// @{

/**
 * @brief   Names and values for all MIDI Control Change Controller Numbers.
 */
namespace MIDI_CC {

constexpr uint8_t Bank_Select                       = 0x00;
constexpr uint8_t Modulation_Wheel                  = 0x01;
constexpr uint8_t Breath_Controller                 = 0x02;
// 03	Undefined
constexpr uint8_t Foot_Controller                   = 0x04;
constexpr uint8_t Portamento_Time                   = 0x05;
constexpr uint8_t Data_Entry_MSB                    = 0x06;
constexpr uint8_t Channel_Volume                    = 0x07;
constexpr uint8_t Balance                           = 0x08;
// 09	Undefined
constexpr uint8_t Pan                               = 0x0A;
constexpr uint8_t Expression_Controller             = 0x0B;
constexpr uint8_t Effect_Control_1                  = 0x0C;
constexpr uint8_t Effect_Control_2                  = 0x0D;
// 0E	Undefined
// 0F	Undefined
constexpr uint8_t General_Purpose_Controller_1      = 0x10;
constexpr uint8_t General_Purpose_Controller_2      = 0x11;
constexpr uint8_t General_Purpose_Controller_3      = 0x12;
constexpr uint8_t General_Purpose_Controller_4      = 0x13;
// 14-1F	Undefined

// LSB

constexpr uint8_t Bank_Select_LSB                   = 0x20 + 0x00;
constexpr uint8_t Modulation_Wheel_LSB              = 0x20 + 0x01;
constexpr uint8_t Breath_Controller_LSB             = 0x20 + 0x02;
// 23	Undefined
constexpr uint8_t Foot_Controller_LSB               = 0x20 + 0x04;
constexpr uint8_t Portamento_Time_LSB               = 0x20 + 0x05;
constexpr uint8_t Data_Entry_MSB_LSB                = 0x20 + 0x06;
constexpr uint8_t Channel_Volume_LSB                = 0x20 + 0x07;
constexpr uint8_t Balance_LSB                       = 0x20 + 0x08;
// 29	Undefined
constexpr uint8_t Pan_LSB                           = 0x20 + 0x0A;
constexpr uint8_t Expression_Controller_LSB         = 0x20 + 0x0B;
constexpr uint8_t Effect_Control_1_LSB              = 0x20 + 0x0C;
constexpr uint8_t Effect_Control_2_LSB              = 0x20 + 0x0D;
// 2E	Undefined
// 2F	Undefined
constexpr uint8_t General_Purpose_Controller_1_LSB  = 0x20 + 0x10;
constexpr uint8_t General_Purpose_Controller_2_LSB  = 0x20 + 0x11;
constexpr uint8_t General_Purpose_Controller_3_LSB  = 0x20 + 0x12;
constexpr uint8_t General_Purpose_Controller_4_LSB  = 0x20 + 0x13;
// 34 - 3F	Undefined

constexpr uint8_t Damper_Pedal                      = 0x40;
constexpr uint8_t Portamento                        = 0x41;
constexpr uint8_t Sostenuto                         = 0x42;
constexpr uint8_t Soft_Pedal                        = 0x43;
constexpr uint8_t Legato_Footswitch                 = 0x44;
constexpr uint8_t Hold_2                            = 0x45;
constexpr uint8_t Sound_Controller_1                = 0x46;
constexpr uint8_t Sound_Controller_2                = 0x47;
constexpr uint8_t Sound_Controller_3                = 0x48;
constexpr uint8_t Sound_Controller_4                = 0x49;
constexpr uint8_t Sound_Controller_5                = 0x4A;
constexpr uint8_t Sound_Controller_6                = 0x4B;
constexpr uint8_t Sound_Controller_7                = 0x4C;
constexpr uint8_t Sound_Controller_8                = 0x4D;
constexpr uint8_t Sound_Controller_9                = 0x4E;
constexpr uint8_t Sound_Controller_10               = 0x4F;
constexpr uint8_t General_Purpose_Controller_5      = 0x50;
constexpr uint8_t General_Purpose_Controller_6      = 0x51;
constexpr uint8_t General_Purpose_Controller_7      = 0x52;
constexpr uint8_t General_Purpose_Controller_8      = 0x53;
constexpr uint8_t Portamento_Control                = 0x54;
// 55-57	Undefined

constexpr uint8_t High_Resolution_Velocity_Prefix   = 0x58;
// 59-5A	Undefined

constexpr uint8_t Effects_1                         = 0x5B;
constexpr uint8_t Effects_2                         = 0x5C;
constexpr uint8_t Effects_3                         = 0x5D;
constexpr uint8_t Effects_4                         = 0x5E;
constexpr uint8_t Effects_5                         = 0x5F;

constexpr uint8_t Data_Increment                    = 0x60;
constexpr uint8_t Data_Decrement                    = 0x61;

constexpr uint8_t NRPN_LSB                          = 0x62;
constexpr uint8_t NRPN_MSB                          = 0x63;
constexpr uint8_t RPN_LSB                           = 0x64;
constexpr uint8_t RPN_MSB                           = 0x65;

// Channel Mode Messages

constexpr uint8_t All_Sound_Off                     = 0x78;
constexpr uint8_t Reset_All_Controllers             = 0x79;
constexpr uint8_t Local_Control                     = 0x7A;
constexpr uint8_t All_Notes_Off                     = 0x7B;
constexpr uint8_t Omni_Mode_Off                     = 0x7C;
constexpr uint8_t Omni_Mode_On                      = 0x7D;
constexpr uint8_t Mono_Mode_On                      = 0x7E;
constexpr uint8_t Poly_Mode_On                      = 0x7F;

}

/// @}

END_CS_NAMESPACE