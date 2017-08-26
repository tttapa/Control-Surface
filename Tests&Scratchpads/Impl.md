# MIDI Controller Implementation

## Class structure

### General
- MIDI_Controller

- MIDI_Element 
    - MIDI_Control_Element
        - Analog
        - AnalogHiRes
        - Digital
        - DigitalLatch
        - RotaryEncoder
    - MIDI_Input_Element
        - MIDI_Input_LED
        - MIDI_Input_VU           (TODO)
        - MIDI_Input_PotRing      (TODO)

- Bank 

- BankSelector

- ExtendedIOElement
    - ShiftRegisterOut
        - SPI_ShiftRegisterOut    (TODO)
    - ShiftRegisterIn             (TODO)
        - SPI_ShiftRegisterIn     (TODO)
    - AnalogMultiplex


### Teensy & Leonardo:
- MIDI_Interface
    - USBMIDI_Interface
    - StreamDebugMIDI_Interface
        - SerialDebugMIDI_Interface
            - HardwareSerialDebugMIDI_Interface
            - USBDebugMIDI_Interface
            - SoftwarSerialDebugMIDI_Interface
    - StreamMIDI_Interface
        - SerialMIDI_Interface
            - HardwareSerialMIDI_Interface
            - USBSerialMIDI_Interface
                - HairlessMIDI_Interface
            - SoftwarSerialMIDI_Interface
            
### UNO & MEGA:
- MIDI_Interface
    - USBMIDI_Interface
    - StreamDebugMIDI_Interface
        - SerialDebugMIDI_Interface
            - HardwareSerialDebugMIDI_Interface
            	- USBDebugMIDI_Interface
            - SoftwarSerialDebugMIDI_Interface
    - StreamMIDI_Interface
        - SerialMIDI_Interface
            - HardwareSerialMIDI_Interface
            	- USBSerialMIDI_Interface
                	- HairlessMIDI_Interface
            - SoftwarSerialMIDI_Interface


***


## Class descriptions

### MIDI_Controller

#### getInstance()
Return singleton instance

#### refresh()
- Refresh all MIDI_Control_Elements
- Read MIDI input
	- Update all MIDI_Input_Elements
- Refresh all MIDI_Input_Elements
- Update all BankSelectors