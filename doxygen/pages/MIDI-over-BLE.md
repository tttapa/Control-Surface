# MIDI over BLE {#md_pages_MIDI-over-BLE}
Control Surface has different MIDI over BLE backends for different
Arduino-compatible boards.

To use MIDI over Bluetooth Low Energy in your code, you usually don't have to
worry about these backends, and you can simply instantiate a
@ref BluetoothMIDI_Interface. For an example, see @ref BLEMIDI-Adapter.ino.

## BLE Backends

A MIDI over BLE backend handles the low-level BLE communication, from setting
up the BLE stack and configuring the GATT characteristics to sending and
receiving MIDI messages over BLE. The backend is used by the
@ref GenericBLEMIDI_Interface class which provides the actual high-level
@ref MIDI_Interface API. The @ref BluetoothMIDI_Interface class uses the default
backend for the particular board.

| Board                               | Default backend            | Other supported backends |
|:------------------------------------|:---------------------------|:-------------------------|
| Arduino Nano 33 IoT                 | @ref ArduinoBLEBackend     |                          |
| Arduino Nano RP2040                 | @ref ArduinoBLEBackend     |                          |
| Arduino Nano ESP32                  | @ref ESP32BluedroidBackend | @ref ESP32NimBLEBackend  |
| Arduino Nano 33 BLE                 | @ref ArduinoBLEBackend     |                          |
| Arduino MKR 1010 WiFi               | @ref ArduinoBLEBackend     |                          |
| Arduino UNO R4 WiFi                 | @ref ArduinoBLEBackend     |                          |
| Arduino GIGA R1 WiFi                | @ref ArduinoBLEBackend     |                          |
| Raspberry Pi Pico W (RP2040)        | @ref BTstackBackgroundBackend |                       |
| ESP32                               | @ref ESP32BluedroidBackend | @ref ESP32NimBLEBackend  |
| ESP32-S3                            | @ref ESP32BluedroidBackend | @ref ESP32NimBLEBackend  |
| ESP32-C3, ESP32-C6, ESP32-H2        | @ref ESP32BluedroidBackend | @ref ESP32NimBLEBackend  |

### ArduinoBLEBackend

Uses the [ArduinoBLE](https://github.com/arduino-libraries/ArduinoBLE) library,
and should support any boards supported by that library.

This is a polling backend: you should call @ref GenericBLEMIDI_Interface::update
regularly to ensure proper operation.

Because of limitations in the ArduinoBLE library, sending large packets is not
possible. The default MTU is set to 23, with no way to negotiate a larger MTU,
or even query the current MTU. This leaves only 20 bytes of MIDI data per packet,
so the throughput is quite poor.

### ESP32BluedroidBackend

This is the default backend for ESP32 boards. It uses the ESP-IDF [Bluedroid](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/bt_le.html)
stack.

The Bluetooth stack runs in its own task, and events are delivered
asynchronously. The sending of messages also happens asynchronously. Calling
@ref GenericBLEMIDI_Interface::update is therefore less critical compared to the
ArduinoBLEBackend, but should still be done regularly to avoid the receiving
buffer from filling up, since there is no BLE flow control.

### ESP32NimBLEBackend

This backend uses the newer [Apache MyNewt NimBLE](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/nimble/index.html)
stack.

Like the @ref ESP32BluedroidBackend, the NimBLE backend uses its own threads and
does not require polling.

The NimBLE ESP-IDF component is disabled by default in the [arduino-esp32](https://github.com/espressif/arduino-esp32)
core, so you'll need to install the [h2zero/NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)
library in order to use this backend.

Defining @ref CS_USE_NIMBLE before including Control Surface changes the default
backend for ESP32 to the @ref ESP32NimBLEBackend.

### BTstackBackgroundBackend

This backend uses the [BTstack](https://github.com/bluekitchen/btstack) stack
that ships with the [pico-sdk](https://github.com/raspberrypi/pico-sdk).

All Bluetooth events are handled asynchronously (using the background
[`async_context`](https://www.raspberrypi.com/documentation/pico-sdk/networking.html#ga092b97e879be5b9aa9121abda23e1337)).
Currently, buffering outgoing MIDI messages still requires polling, although
this could be replaced by a BTstack timer in the future.

To use the @ref BluetoothMIDI_Interface on the Pico W, you have to enable the
Bluetooth stack in the _Tools > IP/Bluetooth Stack_ menu in the IDE: <https://arduino-pico.readthedocs.io/en/latest/bluetooth.html#enabling-bluetooth>
