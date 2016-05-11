#define CMD_START     0xFF
#define CMD_PRESS     0x01
#define CMD_RELEASE   0x02
#define CMD_CHANGEMOD 0x04

#define USB_LED_NUM_LOCK 1
#define USB_LED_CAPS_LOCK 2
#define USB_LED_SCROLL_LOCK 4
#define USB_LED_COMPOSE 8
#define USB_LED_KANA 16

#include "CircularBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif
void usb_keyboard_write(uint8_t c);
void usb_keyboard_write_unicode(uint16_t cpoint);
void usb_keyboard_press_keycode(uint16_t n);
void usb_keyboard_release_keycode(uint16_t n);
void usb_keyboard_release_all(void);
int usb_keyboard_press(uint8_t key, uint8_t modifier);
int usb_keyboard_send(void);
extern uint8_t keyboard_modifier_keys;
extern uint8_t keyboard_media_keys;
extern uint8_t keyboard_keys[6];
extern uint8_t keyboard_protocol;
extern uint8_t keyboard_idle_config;
extern uint8_t keyboard_idle_count;
extern volatile uint8_t keyboard_leds;
#ifdef __cplusplus
}
#endif

uint8_t packet[3];
CircularBuffer<uint8_t> buf;
uint8_t old_leds = 0;

void isrReset() {
  cli();
  Keyboard.releaseAll();
  sei();
}

void setup() {
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(15, isrReset, RISING);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial1.begin(57600);
  Keyboard.begin();
  digitalWrite(13, LOW);
}

void serialEvent1() {
  while(Serial1.available()) {
    buf.add(Serial1.read());
  }
}

void pressRaw(uint8_t key) {
  if(keyboard_keys[0] != key &&
     keyboard_keys[1] != key &&
     keyboard_keys[2] != key && 
     keyboard_keys[3] != key && 
     keyboard_keys[4] != key && 
     keyboard_keys[5] != key) {
    for(int i = 0; i < 6; i++) {
      if(keyboard_keys[i] == 0) {
          keyboard_keys[i] = key;
          break;
      }
    }
    Keyboard.send_now();
  }
}

void releaseRaw(uint8_t key) {
  for(int i = 0; i < 6; i++) {
      if(key != 0 && keyboard_keys[i] == key) {
          keyboard_keys[i] = 0;
      }
  }
  Keyboard.send_now();
}

void loop() {
  if(buf.getNumEntries() >= 3) {
    digitalWrite(13, HIGH);
    buf.remove(packet, 3);
    switch(packet[0]) {
      case CMD_CHANGEMOD:
        Keyboard.set_modifier(packet[2]);
        Keyboard.send_now();
        break;
      case CMD_PRESS:
        pressRaw(packet[2]);
        break;
      case CMD_RELEASE:
        releaseRaw(packet[2]);
        break;
      case CMD_START:
        Keyboard.releaseAll();
        break;
      default:
        break;
    }
    digitalWrite(13, LOW);
  }
  if(keyboard_leds ^ old_leds) {
    old_leds = keyboard_leds;
    // TODO: Send data back over serial
    /*
    if (keyboard_leds & USB_LED_CAPS_LOCK)
    {
        digitalWrite(13, HIGH);
    }
    else
    {
        digitalWrite(13, LOW);
    }
    */
  }
}

