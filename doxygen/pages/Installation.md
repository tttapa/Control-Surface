# Installation {#md_pages_Installation}

## Arduino

This document assumes that the **Arduino IDE** is already installed, as well as 
the necessary hardware support packages, such as Teensyduino or the ESP32 Core, 
if you're not using a standard board.  
See the [Arduino website](https://www.arduino.cc/en/guide) for more details.  

You'll have to install the Control Surface library itself, as well as some 
dependencies.

### 1. Download

Download the repository as a ZIP archive by going to the [home page of the 
repository](https://github.com/tttapa/Control-Surface) and clicking
the green <kbd>Code</kbd> button in the top right, then choosing “Download ZIP”.

Alternatively, click the following direct download link:
<https://github.com/tttapa/Control-Surface/archive/refs/heads/main.zip>

### 2. Install the Library in the Arduino IDE

Open the Arduino IDE, and go to the _Sketch &gt; Include Library &gt; Add .ZIP
Library_ menu.  
Then navigate to your downloads directory where you just downloaded the 
library.  
Select it, and click _Ok_.

### 3. Dependencies

If you are using an Arduino board with on-board USB capabilities, you need the
[**MIDIUSB library**](https://github.com/arduino-libraries/MIDIUSB). 
Install it as explained before, or by using the 
[Library Manager](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library/).

For MIDI over BLE support, some boards may require the installation of the
[**ArduinoBLE library**](https://github.com/arduino-libraries/ArduinoBLE).  
Some of the examples may also depend on any of the following libraries:
[Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306),
[FastLED](https://github.com/FastLED/FastLED),
[Arduino AppleMIDI Library](https://github.com/lathoub/Arduino-AppleMIDI-Library).

# PlatformIO

To use Control Surface in a PlatformIO project, add the following to your
`platformio.ini` configuration:

```ini
lib_deps = https://github.com/tttapa/Control-Surface.git#main
lib_ignore = MIDIUSB
```