#pragma once

// TODO: Teensy 4.0 SoftwareSerial bug
#if defined(__AVR__) || (defined(TEENSYDUINO) && TEENSYDUINO != 147) ||        \
    (defined(TEENSYDUINO) && !defined(__IMXRT1052__) &&                        \
     !defined(__IMXRT1062__))

#include "SerialMIDI_Interface.hpp"
#include <SoftwareSerial.h>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over a SoftwareSerial interface.
 * 
 * @ingroup MIDIInterfaces
 */
class SoftwareSerialMIDI_Interface
    : public SerialMIDI_Interface<SoftwareSerial> {
  public:
    /**
     * @brief   Create a SoftwareSerialMIDI_Interface on the given
     *          SoftwareSerial interface with the given baud rate.
     *
     * @param   serial
     *          The SoftwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     */
    SoftwareSerialMIDI_Interface(SoftwareSerial &serial, unsigned long baud)
        : SerialMIDI_Interface(serial, baud) {}
};

END_CS_NAMESPACE

#endif
