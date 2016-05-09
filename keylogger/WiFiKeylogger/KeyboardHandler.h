#pragma once

// Require keyboard control library
#include "RawKeyboardController.h"

class KeyboardHandler {
  private:
    USBHost usb;
    RawKeyboardController keyboard;
    void (*logCallback)(char *output) = NULL;

    char *pressedStr = "P ";
    char *releasedStr = "R ";
    
    char *keymap[255] = {
      NULL,
      NULL,
      NULL,
      NULL,
      "a",
      "b",
      "c",
      "d",
      "e",
      "f",
      "g",
      "h",
      "i",
      "j",
      "k",
      "l",
      "m",
      "n",
      "o",
      "p",
      "q",
      "r",
      "s",
      "t",
      "u",
      "v",
      "w",
      "x",
      "y",
      "z",
      "1",
      "2",
      "3",
      "4",
      "5",
      "6",
      "7",
      "8",
      "9",
      "0",
      "<Enter>",
      "<Esc>",
      "<Backspace>",
      "<Tab>",
      "<Space>",
      "-",
      "=",
      "[",
      "]",
      "\\",
      "#",
      ";",
      "\'",
      "`",
      ",",
      ".",
      "/",
      "<Caps Lock>",
      "<F1>",
      "<F2>",
      "<F3>",
      "<F4>",
      "<F5>",
      "<F6>",
      "<F7>",
      "<F8>",
      "<F9>",
      "<F10>",
      "<F11>",
      "<F12>",
      "<PrtSc>",
      "<Scroll Lock>",
      "<Pause>",
      "<Insert>",
      "<Home>",
      "<Page Up>",
      "<Delete>",
      "<End>",
      "<Page Down>",
      "<Right>",
      "<Left>",
      "<Down>",
      "<Up>",
      "<Num Lock>",
      "//",
      "*",
      "-",
      "+",
      "<Enter>",
      "1",
      "2",
      "3",
      "4",
      "5",
      "6",
      "7",
      "8",
      "9",
      "0",
      ".",
      "\\",
      "<Application>",
      "<Power>",
      "=",
      "<F13>",
      "<F14>",
      "<F15>",
      "<F16>",
      "<F17>",
      "<F18>",
      "<F19>",
      "<F20>",
      "<F21>",
      "<F22>",
      "<F23>",
      "<F24>",
      "<Execute>",
      "<Help>",
      "<Menu>",
      "<Select>",
      "<Stop>",
      "<Again>",
      "<Undo>",
      "<Cut>",
      "<Copy>",
      "<Paste>",
      "<Find>",
      "<Mute>",
      "<Vol Up>",
      "<Vol Down>",
      "<Caps Lock>",
      "<Num Lock>",
      "<Scroll Lock>",
      ",",
      "=",
      NULL
    };
    uint32_t currentUSBstate;
    uint32_t lastUSBstate;
  public:
    KeyboardHandler() : keyboard(usb) {
      keymap[0xE0] = "<LeftCtrl>";
      keymap[0xE1] = "<LeftShift>";
      keymap[0xE2] = "<LeftAlt>";
      keymap[0xE3] = "<LeftGUI>";
      keymap[0xE4] = "<RightCtrl>";
      keymap[0xE5] = "<RightShift>";
      keymap[0xE6] = "<RightAlt>";
      keymap[0xE7] = "<RightGUI>";
      
      logCallback = &dummyCallback;
    }
    bool init() {
      int ret = usb.Init() != -1;
      //if(!ret) client->print("USB Host did not start.\n");
      //client->print("USB Host started\n");
      return ret;
    }
    void task() {
      // Process USB tasks
      usb.Task();
    
      /*b
      currentUSBstate = usb.getUsbTaskState();
      if (lastUSBstate != currentUSBstate) {
        client->print("USB state changed: 0x");
        client->print(lastUSBstate, HEX);
        client->print(" -> 0x");
        client->print(currentUSBstate, HEX);
        client->print("\n");
        switch (currentUSBstate) {
          case USB_ATTACHED_SUBSTATE_SETTLE: client->print("Device Attached\n"); break;
          case USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE: client->print("Detached, waiting for Device\n"); break;
          case USB_ATTACHED_SUBSTATE_RESET_DEVICE: client->print("Resetting Device\n"); break;
          case USB_ATTACHED_SUBSTATE_WAIT_RESET_COMPLETE: client->print("Reset complete\n"); break;
          case USB_STATE_CONFIGURING: client->print("USB Configuring\n"); break;
          case USB_STATE_RUNNING: client->print("USB Running\n"); break;
        }
        lastUSBstate = currentUSBstate;
      }
      */
    }
    static void dummyCallback(char *str) {
      return;
    }
    int getOemKey() {
      return keyboard.getOemKey();
    }
    int getModifiers() {
      return keyboard.getModifiers();
    }
    int getKey() {
      return keyboard.getKey();
    }
    void setLogCallback(void (*callback)(char*)) {
      logCallback = callback;
    }
    void pressed(char key) {
      logCallback(pressedStr);
      logCallback(keymap[key]);
      logCallback("\r\n");
    }
    void released(char key) {
      logCallback(releasedStr);
      logCallback(keymap[key]);
      logCallback("\r\n");
    }
};
