# WiFiKeylogger
Hardware keylogger dongle with built-in Wi-Fi for live keystroke capture/injection and other fun stuff

## Hardware Setup
Flash WiFiKeylogger.ino to an [Adafruit Feather M0 WiFi](https://www.adafruit.com/product/3010).

Flash KeyboardEmulator.ino to a [Teensy 3.0, 3.1, 3.2 or LC](https://www.pjrc.com/teensy/index.html) (other boards with native USB host may also work).

Connect the 5V and GND pins of the two microcontrollers together. Connect a keyboard to the Feather M0 WiFi and connect the Teensy to the target computer.
