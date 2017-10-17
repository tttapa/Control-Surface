# MIDI Controller Implementation

## Class structure

### General
- Control_Surface

- MIDI_Element
    - MIDI_Control_Element
        - Analog
        - AnalogHiRes
        - Digital
        - DigitalLatch
        - RotaryEncoder
    - MIDI_Input_Element_CC
        - MIDI_Input_7seg_Display
        - MIDI_Input_LED_Ring      ==TODO==
    - MIDI_Input_Element_Note
        - MIDI_Input_LED
    - MIDI_Input_Element_ChannelPressure
        - MIDI_Input_VU
    - MIDI_Input_Element_PitchBend ==TODO==
        - MIDI_Input_Fader	==TODO==
    - MIDI_Input_Element_SysEx	==TODO==

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
    - StreamMIDI_Interface
         - StreamDebugMIDI_Interface
             - SerialDebugMIDI_Interface
                 - HardwareSerialDebugMIDI_Interface
                 - USBDebugMIDI_Interface
                 - SoftwarSerialDebugMIDI_Interface
        - SerialMIDI_Interface
             - HardwareSerialMIDI_Interface
             - USBSerialMIDI_Interface
                 - HairlessMIDI_Interface
             - SoftwarSerialMIDI_Interface

### UNO & MEGA:
- MIDI_Interface
    - StreamMIDI_Interface
        - StreamDebugMIDI_Interface
            - SerialDebugMIDI_Interface
                - HardwareSerialDebugMIDI_Interface
                    - USBDebugMIDI_Interface
            - SoftwarSerialDebugMIDI_Interface
        - SerialMIDI_Interface
            - HardwareSerialMIDI_Interface
                - USBSerialMIDI_Interface
                    - USBMIDI_Interface
                    - HairlessMIDI_Interface
            - SoftwarSerialMIDI_Interface


***


## Class descriptions

### Control_Surface

#### getInstance()
Return singleton instance

#### refresh()
- Refresh all MIDI_Control_Elements
- Read MIDI input
	- Update all MIDI_Input_Elements
- Refresh all MIDI_Input_Elements
- Update all BankSelectors

## Memory:
Finished example - Arduino Leonardo: 
		
				Free RAM	Flash used
 	- 2.1.0 : 	1581B
 	- New:  	972B 		13340B	(normal)
 	- New:  	1208B		13218B	(single byte average)
 	- New:  	1114B		12094B	(no midi input)
 	- New:  	1350B		11972B	(no midi input, single byte average)