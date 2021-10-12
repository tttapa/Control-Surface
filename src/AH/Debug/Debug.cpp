#include "Debug.hpp"

#ifdef DEBUG_OUT
#pragma message("Debugging enabled on output " DEBUG_STR(DEBUG_OUT))

#ifndef ARDUINO
BEGIN_AH_NAMESPACE
const decltype(std::chrono::high_resolution_clock::now()) start_time =
    std::chrono::high_resolution_clock::now();
END_AH_NAMESPACE
#endif

#endif

#if defined(ESP32)
BEGIN_AH_NAMESPACE
std::mutex debugmutex;
END_AH_NAMESPACE
#elif defined(ARDUINO_ARCH_MBED)
BEGIN_AH_NAMESPACE
rtos::Mutex debugmutex;
END_AH_NAMESPACE
#endif