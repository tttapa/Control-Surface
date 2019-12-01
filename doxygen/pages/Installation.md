# Installation

This document assumes that the Arduino IDE is already installed, as well as the
necessary hardware support packages, such as Teensyduino or the ESP32 Core.

You have to install the Control Surface library itself, as well as some 
dependencies.

If you want to keep up to date with the latest developments, or if you want an 
easy way to update, the git install is recommended. Otherwise, you can use the
no git install.

## Installation (no git)

### 1. Download

Download the repository as a ZIP archive by going to the [home page of the 
repository](https://github.com/tttapa/Control-Surface) and click
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

On Ubuntu and other Debian-based distros: 
```sh
sudo apt install git
```

### 1. Browse to your Arduino Libraries folder

Open a terminal window and change the directory to your Arduino folder.

On Linux: 
```sh
mkdir -p ~/Arduino/libraries && cd $_
```

### 2. Clone the Library

```sh
git clone https://github.com/tttapa/Control-Surface
```

### 3. Updating to the latest version

If you installed the library using git, you can easily update it when a new 
version comes out.  
To update to the latest `master` version:

```sh
git pull
```

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