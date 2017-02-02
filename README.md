# MIDI controller
This is a library for creating a MIDI controller using an Arduino board.  
It enables you to easily create MIDI controllers or instruments with very few lines of code. It supports digital inputs, like pushbuttons and switches, analog inputs, like faders, potentiometers, light sensors, and rotary encoders, like jogwheels. 
The library works on pretty much all Arduino and Teensy boards, like the Arduino Uno, Mega, Zero, Due, Leonardo, Micro, Teensy 3.2, ...

It uses a MIDI-over-USB connection to connect to your computer, and can be used in your favourite DJ or DAW software.

## Installation
### Dependencies
If you are using an Arduino with native USB support, like the Leonardo, Due, Zero, Micro ... you'll need to install the MIDIUSB library.
Install it using the Arduino IDE's library manager (Sketch > Include Library > Manage Libraries). Search for 'MIDIUSB', and install it. Alternatively, downoad the library from [GitHub](https://github.com/arduino-libraries/MIDIUSB).  
If you're using a Teensy board, you have to install the Arduino core for Teensy (aka *Teensyduino*). Download it [here](https://www.pjrc.com/teensy/td_download.html), if you haven't already.  
The MIDI\_controller library depends on the [PJRC Encoder library](https://www.pjrc.com/teensy/td_libs_Encoder.html)  for using rotary encoders and jog wheels. Download and install it first.
### Installing the MIDI_controller library
To install this library, download the [.ZIP file](https://github.com/tttapa/MIDI_controller/archive/master.zip). Then open the Arduino IDE, go to Sketch > Include Library > Add .ZIP library, and open the .ZIP file you just downloaded.
### Installing Atmel Flip (Uno and Mega only)
The Arduino Uno and Mega don't have native USB support, so they need extra firmware to be installed to the USB-to-Serial chip in order to work as a MIDI device. To flash this firmware, you have to install a DFU programmer. On Windows, you can use [Atmel Flip](http://www.atmel.com/tools/flip.aspx).
On OSX and Linux, install *dfu-programmer*.  
**Mac:**  
`sudo port install dfu-programmer`

**Linux:**  
`sudo apt-get install dfu-programmer` 

More information can be found [here](https://www.arduino.cc/en/Hacking/DFUProgramming8U2).

## Using the library
### Arduino boards with native USB support
Open one of the example sketches, connect your Arduino, select the right port from the Tools menu, and hit upload.
### Teensy boards
Open one of the example sketches, connect your Teensy, select the right port from the Tools menu, select 'MIDI' as the USB type, and hit upload.
### Arduino Uno or Mega
Open one of the example sketches, connect your Arduino, select the right port from the Tools menu, and hit upload.
When the upload is complete, take a ballpoint pen (or anything conductive) and reset the ATmega16U2 by connecting the reset pin of the 6-pin ISP header (next to the USB connector) to ground, as shown in the following picture.
![DFU-resetting the ATmega16U2](https://www.arduino.cc/en/uploads/Hacking/Uno-front-DFU-reset.png  "DFU-resetting the ATmega16U2")

#### Windows
Download the HIDUINO firmware ([arduino_midi.hex](https://raw.githubusercontent.com/ddiakopoulos/hiduino/master/compiled_firmwares/arduino_midi.hex)) from the [HIDUINO](https://github.com/ddiakopoulos/hiduino) by Dimitri Diakopoulos.  
==TODO==
#### OSX
==TODO==  
(probably just transcoding the Linux Bash scripts to OSX Bash)
#### Linux
Go to the folder where you installed this library (usually ~/Arduino/libraries/MIDI_controller) and go to 'DFU scripts/Linux' and open it in a terminal. Then allow executing of the scripts:  
`chmod +x *`

And run the script to flash the MIDI firmware:  
`./MIDI*`  
Type 'y' to confirm, and enter your root password if necessary.
The script will download and install the firmware to the Arduino.
When the script is finished, unplug the Arduino and plug it back in: your computer should now recognize it as a MIDI device.

Now that the MIDI firmware is running on the USB-to-Serial converter, you can't upload new sketches to the Arduino.
To upload a new sketch, install the Arduino Serial firmware again: put the Arduino into DFU mode again, by connecting the reset pin to ground, and run the second script in the 'DFU scripts/Linux' folder:  
`./Serial*`  
This script will download the Arduino Serial firmware and install it to the Arduino. When it's finished, unplug the Arduino, plug it back in, and upload your new sketch.  
Then you can install the MIDI firmware again.

## Tested boards
I have tested this library with an Arduino Uno, an Arduino Leonardo and a Teensy 3.2.  
If you try this library with a different board, please let me know whether it works or not.
If it doesn't, please open an issue. Thanks!