#include <Keyboard.h>
#include "keylogger.h"

void setup() {
  Serial1.begin(9600);
  Keyboard.begin();
}

void loop() {
  if(Serial1.available() > 0) {
    uint8_t command = Serial1.read();
    switch(command) {
      case CMD_PRESS:
        Keyboard.press(Serial1.read());
        break;
      case CMD_RELEASE:
        Keyboard.release(Serial1.read());
        break;
      default:
        break;
    }
  }
}

