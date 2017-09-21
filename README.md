# MIDI controller
This is a library for creating a MIDI controller using an Arduino board.  
It enables you to easily create MIDI controllers or instruments with very few lines of code. It supports digital inputs, like pushbuttons and switches, analog inputs, like faders, potentiometers, light sensors, and rotary encoders, like jogwheels. 
The library works on pretty much all Arduino and Teensy boards, like the Arduino Uno, Mega, Zero, Due, Leonardo, Micro, Teensy 3.2, ...

It uses a MIDI-over-USB connection to connect to your computer, and can be used in your favourite DJ or DAW software.

## Download
Download the latest stable version [here](https://github.com/tttapa/MIDI_controller/releases/latest).  
Please read the [installation instructions](#installation) first.

## Features
### Analog inputs
The library can take input from a variety of analog devices, such as variable resistors, **potentiometers**, linear faders, light **sensors** ...  
It supports **averaging** the inputs, to get smooth transitions and to reduce noise.  
You have full control over the value **mapping**, control curves, and the range of the inputs.  
**Accuracy up to 10 bits** supported using pitch bend events. 

Analog inputs can be mapped to controls like **volume**, **balance**, **effect parameters**, **EQ** ...

### Digital inputs
The library can be used with a wide range of digital inputs as well, like **pushbuttons**, latching **switches**, footswitches ...
Buttons can be **inverted** if necessary.

Digital inputs can be mapped to controls like transport control (**play/pause/stop/cue/...** buttons), **mute/solo/rec** buttons, **effect enable/disable**, **looping options**, **sample triggers** ... 

### Rotary encoders
Another way to input continuous values is by using **rotary encoders** or **jog wheels**. Unlike potentiometers, they can turn indefinitely, and allow for more precise controls.  
The library has many settings for **accuracy and speed**.  
Different MIDI modes are available for **compatibility** with a huge range of music production, DJ or DAW software.  

Rotary encoders can be mapped to controls like **volume**, **balance**, **effect parameters**, **EQ**, **jog wheels**, **timeline scrolling**, **library scrolling** ...

### MIDI over USB
The library will automatically detect what kind of Arduino-compatible board you are using, and it will use the USB connection for MIDI communication (if available). This means that you can just plug it into your computer or tablet (PC, Mac, Linux, iOS, Android ...), it will be automatically detected as a MIDI device, and you can start playing right away.  
If your board doesn't support MIDI over USB, it will default to the Serial USB interface. More information on the [MIDI over USB Wiki page](https://github.com/tttapa/MIDI_controller/wiki/MIDI-over-USB).  

For maximum flexibility, you can use the **MIDI functions** to send your own custom MIDI events, without having to worry about compatibility issues between different boards.  
The **debug mode** allows you to print the MIDI events in the serial monitor for easier developing, testing and debugging.  
MIDI interfaces on **hardware serial** ports as well as **SoftwareSerial** ports are supported as well, to use the controller with MIDI devices that use a 5-pin DIN MIDI connector.

### Extended IO
Use **analog multiplexers** and **shift registers** to save IO pins. Using the Extended IO classes and the ExtIO namespace, you can treat them as if they were normal input/output pins, without having to worry about the actual implementation.

## New in version 3.0.0
- Completely reworked library structure
- Added more MIDI interface options
- Added debounce for buttons
- Automatically average analog inputs
- Added Bank and BankSelector classes instead of bank() method per class
- Added files containing constants to use Control Change controller names instead of magic numbers
- Automatically refresh all MIDI Control elements at once 
- Added support for analog multiplexers and shift registers using Extended IO classes
## New in version 2.1.0
- Cleaner and more efficient code
- Bank functionality for rotary encoders
- Minor bug fixes
## New in version 2.0.0
- Easier access to debug mode using `USBMidiController.begin(baud, true);`  
- More examples  
- Minor bug fixes  

***

## Installation
### Installing the MIDI_controller library
To install this library, download the [.ZIP file](https://github.com/tttapa/MIDI_controller/archive/master.zip). Then open the Arduino IDE, go to Sketch > Include Library > Add .ZIP library, and open the .ZIP file you just downloaded.
### Dependencies
The MIDI\_controller library depends on the [PJRC Encoder library](https://www.pjrc.com/teensy/td_libs_Encoder.html) for using rotary encoders and jog wheels. Download the .ZIP library from [GitHub](https://github.com/PaulStoffregen/Encoder) and install it.  

If you are using an Arduino with native USB support, like the Leonardo, Due, Zero, Micro ... you'll need to install the MIDIUSB library.
Install it using the Arduino IDE's library manager (Sketch > Include Library > Manage Libraries). Search for 'MIDIUSB', and install it. Alternatively, download the .ZIP library from [GitHub](https://github.com/arduino-libraries/MIDIUSB) and install it.  

If you're using a Teensy board, you have to install the Arduino core for Teensy (aka *Teensyduino*). Download it [here](https://www.pjrc.com/teensy/td_download.html), if you haven't already.  
### Installing Atmel Flip or DFU-programmer (Uno and Mega only)
The Arduino Uno and Mega don't have native USB support, so they need extra firmware to be installed to the USB-to-Serial chip in order to work as a MIDI device. To flash this firmware, you have to install a DFU programmer. On Windows, you can use [Atmel Flip](http://www.atmel.com/tools/flip.aspx). On OSX and Linux, install *dfu-programmer*.  
**Windows:**
Download and install [Atmel Flip](http://www.atmel.com/tools/flip.aspx):
1.  Download the latest Java Runtime Environment from the [Java site](https://java.com/en/download/manual.jsp).  
2.  Run the java installer.  
3.  Click 'install'.  
4.  Close the installer when finished.  
5.  Download 'Flip' from the [Atmel site](http://www.atmel.com/tools/FLIP.aspx).  
6.  Run the installer.  
7.  Click 'next'.  
8.  Accept the terms in the License agreement and click 'next'.  
9.  Click 'next'.  
10.  Click 'install'.  
11.  Click 'next' when completed.  
12.  Click 'finished'.  

**Mac:**  
`sudo port install dfu-programmer`

**Linux:**  
`sudo apt-get install dfu-programmer` 

More information can be found [here](https://www.arduino.cc/en/Hacking/DFUProgramming8U2).

***

## Using the library
### Arduino boards with native USB support
Open one of the example sketches, connect your Arduino, select the right port from the Tools menu, and hit upload.
### Teensy boards
Open one of the example sketches, connect your Teensy, select the right port from the Tools menu, select 'MIDI' (or 'Serial + MIDI') as the USB type, and hit upload.
### Arduino Uno or Mega
Open one of the example sketches, connect your Arduino, select the right port from the Tools menu, and hit upload.
When the upload is complete, take a ballpoint pen (or anything conductive) and reset the ATmega16U2 by connecting the reset pin of the 6-pin ISP header (next to the USB connector) to ground, as shown in the following picture.
![DFU-resetting the ATmega16U2](https://www.arduino.cc/en/uploads/Hacking/Uno-front-DFU-reset.png  "DFU-resetting the ATmega16U2")

#### Windows
Install the drivers for the ATmega16U2:
1.  Make sure you have reset the ATmega16U2 to put it in DFU mode (Device Firmware Update). Read more [here.](https://www.arduino.cc/en/Hacking/DFUProgramming8U2)  
2.  Go to the Control Panel in Windows. Click 'Hardware and Sound'.  
3.  Under 'Devices and Printers', click 'Device Manager'.  
4.  In Device Manager, there is a category 'Other devices'. In this category, there should be something like 'Arduino Uno DFU' or 'ATmega16U2 DFU'. Right click the device, and select 'Update Driver Software'.  
5.  Choose 'Browse my computer for driver software'  
6.  Then hit 'Browse...' and browse to C:\Program Files\Atmel\Flip 3.4.7\usb in the new window.  
7.  Make sure 'Include subfolders' is checked, then click 'Next'.  
8.  In the Windows Security window that pops up, click 'Install'.  
9.  You should get a notification that Windows has successfully updated the driver software for   'AT90USB162'.  
Download the ATmega16U2 firmware:
1.  Download the Arduino firmware from the [Arduino GitHub page](https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/firmwares/atmegaxxu2/arduino-usbserial/): [Arduino-usbserial-atmega16u2-Uno-Rev3.hex](https://raw.githubusercontent.com/arduino/Arduino/master/hardware/arduino/avr/firmwares/atmegaxxu2/arduino-usbserial/Arduino-usbserial-atmega16u2-Uno-Rev3.hex) or [Arduino-usbserial-atmega16u2-Mega2560-Rev3.hex](https://raw.githubusercontent.com/arduino/Arduino/master/hardware/arduino/avr/firmwares/atmegaxxu2/arduino-usbserial/Arduino-usbserial-atmega16u2-Mega2560-Rev3.hex)  
2.  Download [Dimitri Diakopoulos's](http://www.dimitridiakopoulos.com/hiduino.html) HIDUINO firmware from [his GitHub page](https://github.com/ddiakopoulos/hiduino/blob/master/compiled_firmwares/arduino_midi.hex): [arduino_midi.hex](https://raw.githubusercontent.com/ddiakopoulos/hiduino/master/compiled_firmwares/arduino_midi.hex)  
Flash the MIDI firmware:
1.  Open the Flip software.  
2.  Hit CTRL+S to select a device, choose 'ATmega16U2', and click 'OK'.  
3.  Hit CTRL+U, and click 'Open' to open the USB connection.  
4.  Hit CTRL+L to load a HEX file.  
5.  Browse to your Downloads folder and load the 'arduino_midi.hex' file you just downloaded.  
6.  Make sure all 4 boxes on the left hand side are checked, and click 'Run'.  
7.  Once this is finished, you can unplug the Arduino. Now it is no longer in DFU mode.  
8.  Go back to the Device Manager, and check if the Arduino shows up as a MIDI device, under 'Sound, video and game controllers'.  
9.  Now your Arduino is a working MIDI controller!  
10.	 To upload a new program, just repeat these same steps again, but use the Arduino firmware you downloaded previously.  

#### OSX
==TODO==  
(probably just transcoding the Linux Bash scripts to OSX Bash)

#### Linux
Go to the folder where you installed this library (usually ~/Arduino/libraries/MIDI_controller) and go to 'DFU scripts/Linux' and open it in a terminal. Then allow execution of the scripts:  
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

### MIDI over USB
Read more on how different boards use differents methods to send MIDI over USB in the [Wiki](https://github.com/tttapa/MIDI_controller/wiki/MIDI-over-USB).

### Documentation
You can find the entire library documentation with a description of all available functions in the [Wiki](https://github.com/tttapa/MIDI_controller/wiki/Library-documentation).  

### Hardware
In the [Wiki](https://github.com/tttapa/MIDI_controller/wiki/Hardware), you can also find a page that explains how to connect the hardware, like pusbuttons, potentiometers and rotary encoders.  

### Examples
The library includes a whole lot of example sketches that demonstrate the usage of the library. You can find them under File > Examples > Examples from custom libraries > MIDI Controller or in the [examples folder](https://github.com/tttapa/MIDI_controller/tree/master/examples) on GitHub.


## Tested boards
I have tested this library with an Arduino Uno, an Arduino Leonardo and a Teensy 3.2.  
Other users have successfully used the library with an Arduino Mega 2560 and an Arduino Micro.  

Boards that use an FTDI USB-to-Serial chip (like the Nano and some older boards) do not support MIDI over USB, but can still use Serial MIDI.  

If you try this library with a different board, please let me know whether it works or not.
If it doesn't, please open an [issue](https://github.com/tttapa/MIDI_controller/issues). Thanks!
