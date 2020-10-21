# Installation

This document assumes that the **Arduino IDE** is already installed, as well as 
the necessary hardware support packages, such as Teensyduino or the ESP32 Core, 
if you're not using a standard board.  
Here are some links to installation instructions for the Arduino IDE on
[**Linux**](https://tttapa.github.io/Pages/Ubuntu/Software-Installation/Arduino/Arduino-IDE.html), on [**Windows**](https://www.arduino.cc/en/guide/windows), 
and on [**OSX**](https://www.arduino.cc/en/guide/macOSX).  
If you're using an AVR board (e.g. Arduino Uno, Mega, Nano, Leonardo), you need
at least version 1.8.1 of the Arduino AVR Core 
(see Tools > Board > Boards Manager).

You have to install the Control Surface library itself, as well as some 
dependencies.

If you want to keep up to date with the latest developments, or if you want an 
easy way to update, the Git install is recommended. Otherwise, the installation
without Git will be fine as well.

If you're using **PlatformIO**, you can download the library using the PIO 
Library Manager: 
[https://platformio.org/lib/show/6793/Control Surface/installation](https://platformio.org/lib/show/6793/Control%20Surface/installation)

## Installation (No Git)

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

## Installation (Git)

### 0. Install Git

If you haven't already, install Git from https://git-scm.com/downloads or use 
your system's package manager.

On Ubuntu and other Debian-based distros: 
```sh
sudo apt install git
```

### 1. Browse to your Arduino Libraries folder

Open a terminal window and change the directory to your Arduino folder.

In a Linux terminal or Git Bash: 
```sh
mkdir -p ~/Arduino/libraries && cd $_
```

### 2. Clone the Library

```sh
git clone https://github.com/tttapa/Control-Surface
```

### 3. Updating to the latest version

If you installed the library using Git, you can easily update it when a new 
version comes out.  
To update to the latest `master` version:

```sh
git pull
```

## Dependencies

If you are using an Arduino board with on-board USB capabilities, you need the
[**MIDIUSB library**](https://github.com/arduino-libraries/MIDIUSB). 
Install it as explained before, or by using the 
[Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc3).

To use AppleMIDI, you also need the [AppleMIDI](https://github.com/lathoub/Arduino-AppleMIDI-Library) library and its dependencies.