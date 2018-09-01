#pragma once

#include <stdint.h>

struct ShiftRegisterOutRGB {
    /**
     * @brief   The position of the red output pin for 3-color LEDs.  
     *          For the usual RGB configuration, this is 0.
     */
    const static uint8_t redBit; // = 0;
    /**
     * @brief   The position of the green output pin for 3-color LEDs.  
     *          For the usual RGB configuration, this is 1.
     */
    const static uint8_t greenBit; // = 1;
    /**
     * @brief   The position of the blue output pin for 3-color LEDs.  
     *          For the usual RGB configuration, this is 2.
     */
    const static uint8_t blueBit; // = 2;
};