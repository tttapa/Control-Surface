# MIDI over USB {#md_pages_MIDI-over-USB}
There are some differences in MIDI over USB implementation between different types of Arduino-compatible boards. This page provides an overview and some board recommendations if you're planning to build an Arduino MIDI device with MIDI over USB support.
 
## Arduino boards with native USB support
_Arduino Due, Arduino Leonardo, Arduino Micro, Arduino Nano 33 BLE, Arduino Nano 33 IOT, Arduino Zero, Arduino MKR Zero, Arduino MKR1000 ..._  
***
Some of the newer boards have native USB support (i.e. the USB connection goes directly to the main MCU) this means that they can act as a true USB MIDI device. This is based on PluggableUSB, using the [MIDIUSB](https://github.com/arduino-libraries/MIDIUSB) library.  
You just have to @ref md_pages_Installation "install the MIDIUSB library", and upload a Control Surface sketch with a @ref USBMIDI_Interface.  
The computer will then automatically detect the Arduino as a MIDI device.

The MIDIUSB library only supports one MIDI USB virtual cable, while most Teensies support all 16 cables (see below).

## Teensy
_Teensy 2.0, Teensy++ 2.0, Teensy LC, Teensy 3.0, Teensy 3.1, Teensy 3.2, Teensy 3.5, Teensy 3.6, Teensy 4.0, Teensy 4.1_  
***
Teensy boards have native USB support as well. You don't even need to install anything, because MIDI over USB is implemented in the Teensy Core.  
You just need to select the 'MIDI' USB type from Tools > USB Type > MIDI. Other USB types that include MIDI can be used as well.

You can use up to 16 USB MIDI virtual cables over a single USB connection, they will show up as 16 different MIDI devices on your computer. (On a Teensy 2, this number is limited to 4 USB MIDI cables).

## Arduino boards with an ATmega16U2 (ATmega8U2)
_Arduino Uno, Arduino Mega_
***
Some Arduino's that don't have native USB support can still be used as a USB MIDI device.  
This is because they have a second, smaller microcontroller on board, an ATmega16U2 (ATmega8U2 on older boards). This small USB-capable MCU communicates with the main MCU over TTL UART (serial, TX and RX on pins 0 and 1), and also acts as a USB COM port, to communicate with the computer (for programming and for using the Serial Monitor). During normal operation, the ATmega16U2 is just a USB-to-Serial bridge, but you can also program it to be a USB MIDI-to-Serial bridge.  

You can use the [HIDUINO](https://github.com/ddiakopoulos/hiduino) firmware by [Dimitri Diakopoulos](http://www.dimitridiakopoulos.com/hiduino). This flashing process is called a Device Firmware Upgrade (DFU). On Windows, you can use the Atmel Flip tool, on Linux or Mac, you can use `dfu-programmer`. Everything is explained [here](https://github.com/tttapa/MIDI_controller#arduino-uno-or-mega).  
Because you need the ATmega16U2 for uploading new programs to the Arduino, you have to upload your program first, and then flash the MIDI firmware. If you want to change your program, you have to flash the default Serial firmware again, then upload your new program, and finally flash the MIDI firmware.  
That's quite a cumbersome process, especially if you're just trying different settings, tweaking some values or trying to debug your code.  

#### Debug mode
To make this process a little easier, the MIDI Controller library has a debug MIDI interface, that prints the MIDI events to the serial monitor (or other Stream outputs). This means that you don't need the MIDI firmware to see if it's sending the right MIDI messages.  
You can enable the debug mode by instantiating one of the following `*DebugMIDI_Interfaces`:  
- @ref USBDebugMIDI_Interface
- @ref StreamDebugMIDI_Interface
- @ref SerialDebugMIDI_Interface
- @ref HardwareSerialDebugMIDI_Interface
- @ref SoftwareSerialDebugMIDI_Interface

#### Hairless MIDI
Once that's working, you can try it out with real MIDI messages, by using a software tool called [Hairless MIDI<->Serial bridge](http://projectgus.github.io/hairless-midiserial/). This application takes MIDI messages from the serial port, and sends them to a virtual MIDI port on your computer, so you can access it in your DAW or DJ program.  
To use it in the Control Surface library, instantiate a `HairlessMIDI_Interface` at the top of your sketch instead of the `*DebugMIDI_Interface`.   

#### HIDUINO MIDI over USB
Finally, when you know that everything is working the way you want it, you can change the baud rate to the official MIDI baud rate of 31250, by using the `USBMIDI_Interface`, uploading the sketch, and flashing the HIDUINO MIDI firmware using Flip or dfu-programmer.  
You can now just plug it into your computer, and it will be recognized as a MIDI device, you don't need to run Hairless anymore.  

#### USBMidiKliK
The HIDUINO firmware is rather old, and there's a newer alternative that combines the MIDI USB and Serial USB modes in a single firmware: [USBMidiKliK](https://github.com/TheKikGen/USBMidiKliK). This means that you should be able to program the Arduino and use MIDI over USB without swapping firmwares all the time.  
I haven't tried this myself, but it looks promising.

## Arduino boards with a SAMD11
_Arduino Nano Every_
***
Some newer boards use a SAMD11 as the USB-to-TTL chip. In theory, you could also upload custom firmware to this chip, but I've never seen anyone do it.
This [forum thread](https://forum.arduino.cc/index.php?topic=677150.msg4562019#msg4562019) contains some useful information.  

Once you have working SAMD11 MIDI over USB firmware, the usage is the same as the previous section about the ATmega16U2.  
I wouldn't recommend this approach.

## Boards with a single-purpose USB-to-TTL chip  
_Arduino Nano, Arduino Duemilanove, Chinese Uno & Mega clones ..._
***
Whereas the ATmega16U2 chip is programmable, most other USB-to-TTL chips are single-purpose, so you can't flash them with the HIDUINO MIDI firmware.  
These chips include FTDI chips (Nano and Duemilanove) and the CH340G or CP2102 (both popular on Chinese "Arduino" clones).  
While MIDI over USB is not supported on these boards, you can still use Hairless. Just instantiate a `HairlessMIDI_Interface` at the top of your sketch.  

## Espressif boards
_ESP8266, ESP32_
***
The ESP8266 and ESP32 microcontrollers don't have native USB support, and all development boards I've come across use a single-purpose USB-to-TTL chip, which means that they fall into the same category as the Arduino Nano when it comes to MIDI over USB.

That being said, both the ESP8266 and the ESP32 have built-in WiFi, so you can use rtpMIDI, using the [AppleMIDI library](https://github.com/lathoub/Arduino-AppleMIDI-Library), for example.  
An alternative is to use Open Sound Control (OSC). This is not MIDI, it's an entirely different protocol, built on top of UDP. It can be used for bidirectional communication between audio software and control surfaces. I've successfully used this to build a control surface for Reaper.

Control Surface supports AppleMIDI if you install the AppleMIDI library. See the @ref AppleMIDI.ino example for details.  
OSC is not directly supported by the Control Surface library, but they can be integrated relatively easily.

The ESP32 also has Bluetooth support, so you can use MIDI over BLE. This is supported by the Control Surface library using the @ref BluetoothMIDI_Interface.

***

## Conclusion
I strongly recommend getting a Teensy 3.x or 4.x for building a Control Surface. 
MIDI over USB is supported right out of the box, and it's the only platform that 
currently supports USB audio output. 
On top of that, it has plenty of memory to create large MIDI Controllers, and 
drive lots of displays.

Arduino boards using the nRF52840 or SAMD21 also support MIDI over USB natively.
They're not as powerful as a Teensy, and the SAMD21 doesn't have that much 
memory, but they are suitable for medium-sized MIDI projects.  
This category includes Arduino Nano 33 and Arduino MKR boards.

If you just want a small and cheap MIDI Controller, the Arduino Leonardo/Micro 
are good choices as well, just keep in mind that they don't have a lot of RAM, 
and are pretty slow compared to Teensy boards, both in terms of CPU power and 
IO or analog inputs.

The Arduino Uno, Mega, Nano and Nano Every boards are just not fit for MIDI over
USB, and I wouldn't recommend them at all for this application. They can of 
course still be used if you just want 5-pin DIN MIDI instead of MIDI over USB.

You don't have to worry too much about the number of IO pins a board has, since
you can use multiplexers and GPIO expanders, using the ExtendedIO classes of the 
library.
Memory can be a much bigger problem, especially if you add a display.
