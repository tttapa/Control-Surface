# MIDI controller
This is a library for creating a MIDI controller using an Arduino board.  
It enables you to easily create MIDI controllers or instruments with very few lines of code. It supports digital inputs, like pushbuttons and switches, analog inputs, like faders, potentiometers, light sensors, and rotary encoders, like jogwheels. 
The library works on pretty much all Arduino and Teensy boards, like the Arduino Uno, Mega, Zero, Due, Leonardo, Micro, Teensy 3.2, ...

It uses a MIDI-over-USB connection to connect to your computer, and can be used in your favourite DJ or DAW software.

## Features
### Analog inputs
The library can take input from a variety of analog devices, such as variable resistors, **potentiometers**, linear faders, light **sensors** ...  
It supports **averaging** the inputs, to get smooth transitions and to reduce noise.  
You have full control over the value **mapping**, control curves, and the range of the inputs.  
You can create **banks** of analog inputs, based on the state of a switch. For example, if you have 4 faders connected, you can set them up so that they control the volume of channels 1-4 when the bank select switch is turned off, and channels 5-8 when the bank select switch is on.  
Using the _ControlChange_ class, you can use data from **digital sensors** or from different interfaces as well.  

Analog inputs can be mapped to controls like **volume**, **balance**, **effect parameters**, **EQ** ...

### Digital inputs
The library can be used with a wide range of digital inputs as well, like **pushbuttons**, latching **switches**, footswitches ...
Just like analog inputs, digital inputs can be arranged in **banks**, to control a large number of controls, with a limited number of physical buttons.  
Buttons can be **inverted** if necessary.

Digital inputs can be mapped to controls like transport control (**play/pause/stop/cue/...** buttons), **mute/solo/rec** buttons, **effect enable/disable**, **looping options**, **sample triggers** ... 

### Rotary encoders
Another way to input continuous values is by using **rotary encoders** or **jog wheels**. Unlike potentiometers, they can turn indefinitely, and allow for more precise controls.  
The library has many settings for **accuracy and speed**.  
Different MIDI modes are available for **compatibility** with a huge range of music production, DJ or DAW software.

### MIDI over USB
The library will automatically detect what kind of Arduino-compatible board you are using, and it will use the USB connection for MIDI communication (if available). This means that you can just plug it into your computer or tablet (PC, Mac, Linux, iOS, Android ...), it will be automatically detected as a MIDI device, and you can start playing right away.  
If your board doesn't support MIDI over USB, it will default to the Serial protocol.  

For maximum flexibility, you can use the **MIDI functions** to send your own custom MIDI events, without having to worry about compatibility issues between different boards.  

## Installation
### Dependencies
If you are using an Arduino with native USB support, like the Leonardo, Due, Zero, Micro ... you'll need to install the MIDIUSB library.
Install it using the Arduino IDE's library manager (Sketch > Include Library > Manage Libraries). Search for 'MIDIUSB', and install it. Alternatively, download the library from [GitHub](https://github.com/arduino-libraries/MIDIUSB).  
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
To install and use Atmel Flip on Windows (tested on Vista, 8 and 10):

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
13.  Now, connect the Arduino to your computer, and use a small screwdriver to connect the reset pin of the second ICSP header to the ground. This is the 6-pin ICSP header close to the USB port. (see image)  
    The ATmega16U2 is now in DFU mode (Device Firmware Update). Read more [here.](https://www.arduino.cc/en/Hacking/DFUProgramming8U2)  
14.  Go to the Control Panel in Windows. Click 'Hardware and Sound'.  
15.  Under 'Devices and Printers', click 'Device Manager'.  
16.  In Device Manager, there is a category 'Other devices'. In this category, there should be something like 'Arduino Uno DFU' or 'ATmega16U2 DFU'. Right click the device, and select 'Update Driver Software'.  
17.  Choose 'Browse my computer for driver software'  
18.  Then hit 'Browse...' and browse to C:\Program Files\Atmel\Flip 3.4.7\usb in the new window.  
19.  Make sure 'Include subfolders' is checked, then click 'Next'.  
20.  In the Windows Security window that pops up, click 'Install'.  
21.  You should get a notification that Windows has successfully updated the driver software for   'AT90USB162'.  
22.  Download the Arduino firmware from the [Arduino GitHub page](https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/firmwares/atmegaxxu2/arduino-usbserial/Arduino-usbserial-atmega16u2-Uno-Rev3.hex) by clicking 'RAW' and then CTRL+S to save.  
23.  Change the location to your Downloads folder, delete the .txt extension, and choose 'All files' (keep the .hex extension). Then hit 'Save'.  
24.  Download [Dimitri Diakopoulos's](http://www.dimitridiakopoulos.com/hiduino.html) HIDUINO firmware from [his GitHub page](https://github.com/ddiakopoulos/hiduino/blob/master/compiled_firmwares/arduino_midi.hex). Again, hit 'RAW', CTRL+S, delete the .txt extension, choose 'All files' and save it. Or just download it directly: [arduino_midi.hex](https://raw.githubusercontent.com/ddiakopoulos/hiduino/master/compiled_firmwares/arduino_midi.hex)  
25.  Open the Flip software.  
26.  Hit CTRL+S to select a device, choose 'ATmega16U2', and click 'OK'.  
27.  Hit CTRL+U, and click 'Open' to open the USB connection.  
28.  Hit CTRL+L to load a HEX file.  
29.  Browse to your Downloads folder and load the 'arduino_midi.hex' file you downloaded in step 24.  
30.  Make sure all 4 boxes on the left hand side are checked, and click 'Run'.  
31.  Once this is finished, you can unplug the Arduino. Now it is no longer in DFU mode.  
32.  Go back to the Device Manager, and check if the Arduino shows up as a MIDI device, under 'Sound, video and game controllers'.  
33.  Now your Arduino is a working MIDI controller!  
34.	To upload a new program, just repeat steps 25 - 31 again, but use the Arduino firmware you downloaded in step 23.  

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
