
// =============== AUTOMATICALLY GENERATED FILE: DO NOT EDIT! =============== //
//                                                                            //
// Edit MCUNameFromNoteNumber.py instead.                                     //

#pragma once

#include <AH/Arduino-Wrapper.h>  // FlashString_t
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/// @addtogroup    MCU_Notes
/// @{

/**
 * @brief   Convert a note number to the name of the corresponding Mackie 
 * Control Universal function.
 *
 * @param   note 
 *          The note number of the function to look up.
 */
FlashString_t getMCUNameFromNoteNumber(uint8_t note);

/// @}

} // namespace MCU 

END_CS_NAMESPACE
