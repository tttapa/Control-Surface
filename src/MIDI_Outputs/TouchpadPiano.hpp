#pragma once

#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>
#include <AH/Timing/MillisMicrosTimer.hpp>

BEGIN_CS_NAMESPACE

#define NB_NOTES           12
#define UPDATE_RATE        50
#define HOLD_THRESHOLD_MS 500

namespace PianoMode {
    /**
    * @brief   An enumeration of the different piano modes.
    */
    enum Mode: uint8_t {
        /**
        * @brief Default piano behaviour.
        *        Press and release pads to play and stop notes.
        */
        Standard,
        /**
        * @brief Maintains last note or chord played on the piano.
        */
        Hold,
        /**
        * @brief Only one note is played, the highest tone prevails.
        */
        Monodic
    };
} // namespace PianoMode

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a 12 notes
 *          **TouchPad Piano**, and send out MIDI **Note** events.
 * 
 * It handles Touchpad piano using SCL and sda pins.
 * The supported KeyMap is the following:
 * ```
 *       ___________________________
 *      |  | | | |  |  | | | | | |  |
 *      |  |1| |2|  |  |3| |4| |5|  |
 *      |  '-' '-'  |  '-' '-' '-'  |
 *      |   |   |   |   |   |   |   |
 *      | 6 | 7 | 8 | 9 | 10| 11| 12|
 *      '---'---'---'---'---'---'---'
 * ```
 * It can be configured to different modes @ref PianoMode::Mode
 * This version cannot be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class TouchpadPiano : public MIDIOutputElement {
public:
    /**
     * @brief   Create a new TouchpadPiano object with the given sclPin, sdaPin,
     *          base note and channel and optionally the piano mode.
     * 
     * @param   sclPin
     *          The output clock pin.
     * @param   sdaPin
     *          The input data pin. 
     * @param   baseAddress
     *          The MIDI address of the first pad, containing the note
     *          number [0, 127], channel [Channel_1, Channel_16], and optional 
     *          cable number [Cable_1, Cable_16].
     * @param   mode
     *          The behaviour of the piano. Values are in PianoMode::Mode enum.
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    TouchpadPiano(const pin_t sclPin, const pin_t sdaPin,
                    MIDIAddress baseAddress,
                    PianoMode::Mode mode = PianoMode::Standard,
                    uint8_t velocity = 0x7F)
        : sclPin(sclPin), sdaPin(sdaPin), baseAddress(baseAddress),
          updateTimer(UPDATE_RATE), mode(mode), monodicNote(-1),
          sender(velocity) {}

public:
    void begin() final override {
        // Configure the clock and data pins.
        pinMode(sclPin, OUTPUT);
        pinMode(sdaPin, INPUT);
    }

    void update() final override {
        // Too high rate reading is no working.
        if (!updateTimer) return;

        uint8_t newKeysState[NB_NOTES];
        int8_t newMonodicNote = -1;

        readData(newKeysState, &newMonodicNote);
        
        switch (mode) {
            case PianoMode::Standard:
                handleStandard(newKeysState);
                break;
            case PianoMode::Hold:
                handleHold(newKeysState);
                break;
            case PianoMode::Monodic:
                handleMonodic(newMonodicNote);
                break;
            default:
                break;
        }

        updateTimer.beginNextPeriod();
    }

    /// Set the piano mode
    void setMode(PianoMode::Mode mode) {
        // Leaving Hold mode requires to stop notes.
        if (this->mode == PianoMode::Hold)
        {
            for (uint8_t i = 0; i < NB_NOTES; i++)
            {
                if (keysState[i] == 1)
                {
                    sender.sendOff(baseAddress + i);
                    keysState[i] = 0;
                }
            }
        }
        this->mode = mode;
    }

    /// Get the piano mode
    PianoMode::Mode getMode() { return mode; }

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }

    /// Get the MIDI address.
    MIDIAddress getBaseAddress() const { return this->baseAddress; }
    /// Set the MIDI address. Has unexpected consequences if used while the
    /// touchpad is playing notes.
    void setBaseAddressUnsafe(MIDIAddress address){ this->baseAddress=address; }

private:
    void readData(uint8_t newKeysState[NB_NOTES], int8_t* newMonodicNote)
    {
        holdPrevTouch = holdLastTouchMs;
        for (uint8_t i = 0; i < NB_NOTES; i++)
        {
            digitalWrite(sclPin, LOW);
            newKeysState[NoteMap[i]] = !digitalRead(sdaPin);
            if (newKeysState[NoteMap[i]])
            {
                // For Hold mode, remember the last touch timestamp.
                holdLastTouchMs = millis();
                // For Monodic mode, keep only the highest tone played.
                if (NoteMap[i] > *newMonodicNote)
                    *newMonodicNote = NoteMap[i];
            }
            digitalWrite(sclPin, HIGH);
        }
    }

    void handleStandard(uint8_t newKeysState[NB_NOTES])
    {
        for (uint8_t i = 0; i < NB_NOTES; i++)
        {
            // Key changed state.
            if (newKeysState[i] != keysState[i])
            {
                if (newKeysState[i] == 1)
                {
                    sender.sendOn(baseAddress + i);
                    keysState[i] = 1;
                }
                else
                {
                    sender.sendOff(baseAddress + i);
                    keysState[i] = 0;
                }
            }
#ifdef DEBUG_VERBOSE
            Serial.print(keysState[i]);
#endif
        }
#ifdef DEBUG_VERBOSE
        Serial.println();
#endif
    }

    void handleHold(uint8_t newKeysState[NB_NOTES])
    {
        for (uint8_t i = 0; i < NB_NOTES; i++)
        {
            if ((newKeysState[i] & keysState[i]) == 1)
            {
                // Re-trigger the note if it was already ON.
                if ((holdLastTouchMs - holdPrevTouch) > HOLD_THRESHOLD_MS)
                {
                    sender.sendOff(baseAddress + i);
                    sender.sendOn(baseAddress + i);
                }
            }

            // Key changed state.
            if (newKeysState[i] != keysState[i])
            {
                if (newKeysState[i] == 1)
                {
                    sender.sendOn(baseAddress + i);
                    keysState[i] = 1;
                }
                else
                {
                    if ((holdLastTouchMs - holdPrevTouch) > HOLD_THRESHOLD_MS)
                    {
                        sender.sendOff(baseAddress + i);
                        keysState[i] = 0;
                    }
                }
            }
#ifdef DEBUG_VERBOSE
            Serial.print(keysState[i]);
#endif
        }
#ifdef DEBUG_VERBOSE
        Serial.println();
#endif
    }

    void handleMonodic(int8_t newMonodicNote)
    {
        if (newMonodicNote != monodicNote)
        {
            sender.sendOff(baseAddress + monodicNote);
            if (newMonodicNote != -1)
                sender.sendOn(baseAddress + newMonodicNote);
            monodicNote = newMonodicNote;
        }
#ifdef DEBUG_VERBOSE
        Serial.println(monodicNote);
#endif
    }

private:
    const pin_t sclPin;
    const pin_t sdaPin;
    MIDIAddress baseAddress;
    const uint8_t NoteMap[NB_NOTES] = {1,3,6,8,10,0,2,4,5,7,9,11};
    uint8_t keysState[NB_NOTES] = {0};
    uint64_t holdLastTouchMs = 0;
    uint64_t holdPrevTouch = 0;
    AH::Timer<millis> updateTimer;
    PianoMode::Mode mode;
    int8_t monodicNote;

public:
    DigitalNoteSender sender;
};

END_CS_NAMESPACE
