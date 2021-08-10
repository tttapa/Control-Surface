#pragma once

// TODO: Teensy 4.0 SoftwareSerial bug
#if defined(__AVR__) || (defined(TEENSYDUINO) && TEENSYDUINO != 147) ||        \
    (defined(TEENSYDUINO) && !defined(__IMXRT1052__) &&                        \
     !defined(__IMXRT1062__))

#include "DebugMIDI_Interface.hpp"
#include <SoftwareSerial.h>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a SoftwareSerial interface.
 * 
 * @ingroup MIDIInterfaces
 */
class SoftwareSerialDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<SoftwareSerial> {
  public:
    /**
     * @brief   Construct a SoftwareSerialDebugMIDI_Interface on the given 
     *          SoftwareSerial interface with the given baud rate.
     * 
     * @param   serial
     *          The SoftwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     * @param   prefix
     *          An optional string to print before each message.
     */
    SoftwareSerialDebugMIDI_Interface(SoftwareSerial &serial,
                                      unsigned long baud,
                                      const char *prefix = nullptr)
        : SerialDebugMIDI_Interface(serial, baud, prefix) {}
    /// @see @ref SoftwareSerialDebugMIDI_Interface(SoftwareSerial &,unsigned long,const char *)
    SoftwareSerialDebugMIDI_Interface(SoftwareSerial &serial,
                                      const char *prefix)
        : SerialDebugMIDI_Interface(serial, prefix) {}
};

/**
 * @brief   A class for debug MIDI outputs sending 
 *          human-readable MIDI messages over a SoftwareSerial interface.
 * 
 * @ingroup MIDIInterfaces
 */
class SoftwareSerialDebugMIDI_Output
    : public SerialDebugMIDI_Output<SoftwareSerial> {
  public:
    /**
     * @brief   Construct a SoftwareSerialDebugMIDI_Output on the given 
     *          SoftwareSerial interface with the given baud rate.
     * 
     * @param   serial
     *          The SoftwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     * @param   prefix
     *          An optional string to print before each message.
     */
    SoftwareSerialDebugMIDI_Output(SoftwareSerial &serial, unsigned long baud,
                                   const char *prefix = nullptr)
        : SerialDebugMIDI_Output(serial, baud, prefix) {}
};

END_CS_NAMESPACE

#endif