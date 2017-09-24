# MIDI over USB
There are some differences in MIDI over USB implementation between different types of Arduino-compatible boards.
 
## Arduino boards with native USB support
_Arduino Due, Arduino Zero, Arduino Leonardo, Arduino Micro, Arduino LilyPad USB ..._  
***
Some of the newer boards have native USB support (i.e. the USB connection goes directly to the main MCU) this means that they can pretend to be a USB MIDI device. This is based on PluggableUSB, using the [MIDIUSB](https://github.com/arduino-libraries/MIDIUSB) library.  
All you have to do to get it working, is installing the library, and upload a sketch.  
The computer will then automatically detect the Arduino as a MIDI device.

## Teensy
_Teensy 2.0, Teensy++ 2.0, Teensy LC, Teensy 3.0, Teensy 3.1, Teensy 3.2, Teensy 3.5, Teensy 3.6_  
***
Teensy boards have native USB support as well. You don't even need to install anything, because MIDI over USB is implemented in the Teensy Core.  
You just need to select the 'MIDI' USB type from Tools > USB Type > MIDI.

## Arduino boards with an ATmega16U2 (ATmega8U2)
_Arduino Uno, Arduino Mega_
***
Some Arduino's that don't have native USB support can still be used as a USB MIDI device.  
This is because they have a second, smaller microcontroller on board, an ATmega16U2 (ATmega8U2 on older boards), that communicates with the main MCU over TTL UART (serial, TX and RX on pins 0 and 1), and also acts as a USB COM port, to communicate with the computer (for programming and for using the Serial Monitor). During normal operation, the ATmega16U2 is just a USB-to-Serial bridge, but you can also program it to be a USB MIDI-to-Serial bridge.  
You can flash the [HIDUINO](https://github.com/ddiakopoulos/hiduino) firmware by [Dimitri Diakopoulos](http://www.dimitridiakopoulos.com/hiduino). This flashing process is called a DFU, or Device Firmware Upgrade. You can use the Atmel Flip tool on Windows, or dfu-programmer on Linux and Mac. Everything is explained in the [ReadMe](https://github.com/tttapa/MIDI_controller/blob/master/README.md).  
Because you need the ATmega16U2 for uploading new programs to the Arduino, you have to upload your program first, and then flash the MIDI firmware. If you want to change your program, you have to flash the default Serial firmware again, then upload your new program, and finally flash the MIDI firmware.  
That's quite a cumbersome process, especially if you're just trying different settings, tweaking some values or trying to debug your code.  
#### Debug mode
To make this process a little easier, the MIDI Controller library has a debug MIDI interface, that prints the MIDI events to the serial monitor (or other Stream outputs). This means that you don't need the MIDI firmware to see if it's sending the right MIDI messages.  
You can enable the debug mode by instantiating a DebugMIDI_Interface:  
`StreamDebugMIDI_Interface(Stream &stream)`  
`SerialDebugMIDI_Interface(Serial_t &serial, unsigned long baud)`  
`HardwareSerialDebugMIDI_Interface(HardwareSerial &serial, unsigned long baud)`  
`USBDebugMIDI_Interface(unsigned long baud)`  
`SoftwarSerialDebugMIDI_Interface(SoftwareSerial &serial, unsigned long baud)` 
#### Hairless MIDI
Once that's working, you can try it out with real MIDI messages, by using a software tool called [Hairless MIDI<->Serial bridge](http://projectgus.github.io/hairless-midiserial/). This application takes MIDI messages from the serial port, and sends them to a virtual MIDI port on your computer, so you can access it in your DAW or DJ program.  
To use the MIDI Controller library, instantiate a `HairlessMIDI_Interface` at the top of your sketch.    
#### HIDUINO MIDI over USB
Finally, when you know that everything is working fine, you can change the baud rate to the official MIDI baud rate of 31250, by using the default `USBMIDI_Interface` (don't explicitly instantiate any interfaces) uploading the sketch, and flashing the HIDUINO MIDI firmware using Flip or dfu-programmer.  
You can now just plug it into your computer, and it will be recognized as a MIDI device, you don't need to run Hairless anymore.  

## Boards with a single-purpose USB-to-TTL chip  
_Arduino Nano, Arduino Duemilanove, Chinese Uno & Mega clones ..._
***
Whereas the ATmega16U2 chip is programmable, most other USB-to-TTL chips are single-purpose, so you can't flash them with the HIDUINO MIDI firmware.  
These chips include FTDI chips (Nano and Duemilanove) and the CH340G or CP2102 (both very popular on Chinese "Arduino" clones).  
While MIDI over USB is not supported on these boards, you can still use Hairless. Just instantiate a `HairlessMIDI_Interface` at the top of your sketch.    

