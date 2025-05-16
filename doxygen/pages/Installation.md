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

With some boards, if you get a compile error mentioning you should set "Tools->Pin Numbering set to "By GPIO number (legacy)", include this line in your `platformio.ini` configuration:
```ini
build_unflags = -DBOARD_HAS_PIN_REMAP
```

This is a full working example for Arduino Nano ESP32 (tested on the compatible Waveshare ESP32-S3-Nano):
`platformio.ini`:
```ini
[env:arduino_nano_esp32]
platform = espressif32
board = arduino_nano_esp32
framework = arduino

build_flags =
  -D ARDUINO_USB_MODE=1
  -D ARDUINO_USB_CDC_ON_BOOT=1
  -D ESP32_REMAP_DISABLED=1

build_unflags = -DBOARD_HAS_PIN_REMAP

lib_deps = https://github.com/tttapa/Control-Surface.git#main
lib_ignore = MIDIUSB
```

Example `main.cpp`, led lights up if user plays C4 on a computer connected over bluetooth midi to the microcontroller.
```c++
#include <Arduino.h>
#include <Control_Surface.h>
#include <MIDI_Interfaces/BluetoothMIDI_Interface.hpp> 

BluetoothMIDI_Interface midi;

NoteLED led { LED_BUILTIN, MIDI_Notes::C[4] };

void setup() {
  delay(1000); 
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
```
