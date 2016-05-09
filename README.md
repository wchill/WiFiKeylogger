# WiFiKeylogger
Hardware keylogger dongle with built-in Wi-Fi for live keystroke capture/injection and other fun stuff

## Hardware Setup
Flash WiFiKeylogger.ino to an [Adafruit Feather M0 WiFi](https://www.adafruit.com/product/3010). Note that instead of using Adafruit's WiFi library, a [specially modified version](https://github.com/wchill/Adafruit_WINC1500) by me must be used instead due to performance reasons (the standard library performs all calls as blocking operations). Make sure to change the WiFi SSID, password, and IP/port as necessary.

Flash KeyboardEmulator.ino to a [Teensy 3.0, 3.1, 3.2 or LC](https://www.pjrc.com/teensy/index.html) (other boards with keyboard emulation support may also work with some modification).

Connect the 5V and GND pins of the two microcontrollers together, then wire the Feather M0 WiFi's TX pin to the Teensy's RX pin and vice versa. Connect a keyboard to the Feather M0 WiFi and connect the Teensy to the target computer.
