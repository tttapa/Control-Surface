# MIDI Controller Implementation

## Class structure

### General
- MIDI\_Controller\_ (singleton)

- MIDI\_Element
    - MIDI\_Control\_Element
        - Analog
        - AnalogHiRes
        - ButtonMatrix
        - Digital
        - DigitalLatch
        - RotaryEncoder

- Bank 

- BankSelector

- ExtendedIOElement
    - ShiftRegisterOut
        - SPI\_ShiftRegisterOut    (TODO)
    - ShiftRegisterIn             (TODO)
        - SPI\_ShiftRegisterIn     (TODO)
    - AnalogMultiplex


### Teensy & Leonardo:
- MIDI\_Interface
    - USBMIDI\_Interface
    - StreamMIDI\_Interface
         - StreamDebugMIDI\_Interface
             - SerialDebugMIDI\_Interface
                 - HardwareSerialDebugMIDI\_Interface
                 - USBDebugMIDI\_Interface
                 - SoftwarSerialDebugMIDI\_Interface
        - SerialMIDI\_Interface
             - HardwareSerialMIDI\_Interface
             - USBSerialMIDI\_Interface
                 - HairlessMIDI\_Interface
             - SoftwarSerialMIDI\_Interface

### UNO & MEGA:
- MIDI\_Interface
    - StreamMIDI\_Interface
        - StreamDebugMIDI\_Interface
            - SerialDebugMIDI\_Interface
                - HardwareSerialDebugMIDI\_Interface
                    - USBDebugMIDI\_Interface
            - SoftwarSerialDebugMIDI\_Interface
        - SerialMIDI\_Interface
            - HardwareSerialMIDI\_Interface
                - USBSerialMIDI\_Interface
                    - USBMIDI\_Interface
                    - HairlessMIDI\_Interface
            - SoftwarSerialMIDI\_Interface


***


## Class descriptions

### MIDI\_Controller

#### getInstance()
Return singleton instance

#### refresh()
- If it's the first time, call `begin()`
- Refresh all MIDI\_Control\_Elements
- Update all BankSelectors
- Read and discard MIDI input

... 

***

(TODO)