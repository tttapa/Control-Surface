# Installation

This document assumes that the Arduino IDE is already installed, as well as the
necessary hardware support packages, such as Teensyduino.

## Installation (no git)

### 1. Download
Download the repository as a ZIP archive by going to the [home page of the 
repository](https://github.com/tttapa/Control-Surface/tree/revision-4) and click
the green _Clone or download_ button on the top right.

### 2. Install the Library in the Arduino IDE
Open the Arduino IDE, and go to the _Sketch &gt; Include Library &gt; Add .ZIP
Library_ menu.  
Then navigate to your downloads directory where you just downloaded the 
library.  
Select it, and click _Ok_.

## Installation (git)

### 0. Install git
If you haven't already, install git from https://git-scm.com/downloads or use 
your system's package manager.

Ubuntu and other Debian-based distros: `sudo apt-get install git`

### 1. Browse to your Arduino Libraries folder
Open a terminal window and change the directory to your Arduino folder.

Linux: `mkdir -p ~/Arduino/libraries && cd $_`

### 2. Clone the Library
`git clone -b revision-4 --single-branch
https://github.com/tttapa/Control-Surface`

## Dependencies
If you are using an Arduino board with on-board USB capabilities, you need the
[MIDIUSB library](https://github.com/arduino-libraries/MIDIUSB).  
Install it as explained before, or by using the [Library Manager](
https://www.arduino.cc/en/Guide/Libraries#toc3).

The Control Surface Library depends on the [PJRC Encoder library](
https://www.pjrc.com/teensy/td_libs_Encoder.html) for reading the
input from rotary encoders. Download and install it if you plan on using those.

If you are using a Teensy, you don't need the MIDIUSB library, and the Encoder
library should already be installed.