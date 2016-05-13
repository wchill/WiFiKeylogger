#pragma once

// Require keyboard control library
#include "RawKeyboardController.h"

/* Set up watchdog in case usb.Task() hangs
 * Taken from https://forum.arduino.cc/index.php?topic=366836.0
 */

static void WDTsync() {
  while (WDT->STATUS.bit.SYNCBUSY == 1); //Just wait till WDT is free
}

void resetWDT() {
  // reset the WDT watchdog timer.
  // this must be called before the WDT resets the system
  WDT->CLEAR.reg= 0xA5; // reset the WDT
  WDTsync(); 
}

void systemReset() {
  // use the WDT watchdog timer to force a system reset.
  // WDT MUST be running for this to work
  WDT->CLEAR.reg= 0x00; // system reset via WDT
  WDTsync(); 
}

void setupWDT(uint8_t period) {
  // initialize the WDT watchdog timer

  WDT->CTRL.reg = 0; // disable watchdog
  WDTsync(); // sync is required

  // Change watchdog's clock to 32.768kHz slow clock
  GCLK->CLKCTRL.reg = uint16_t(
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK2 |
    GCLK_CLKCTRL_ID(3)
  );

  WDT->CONFIG.reg = min(period,11); // see Table 17-5 Timeout Period (valid values 0-11)
                                    // 2 ^ (period + 3) clock cycles, so period of 11 = 16384 clock cycles
                                    // Using 32.768kHz slow clock, we get timeout of 500ms

  WDT->CTRL.reg = WDT_CTRL_ENABLE;  // enable watchdog
  WDTsync(); 
}

class KeyboardHandler {
  private:
    USBHost usb;
    RawKeyboardController keyboard;
    void (*logCallback)(char *output) = NULL;

    char *pressedStr = "P ";
    char *releasedStr = "R ";
    uint32_t currentUSBstate;
    uint32_t lastUSBstate;

    #include "keymap.h"
    
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
      if(!ret) logCallback("USB Host did not start.\n");
      else logCallback("USB Host started\n");
      return ret;
    }
    void task() {
      // Process USB tasks
      usb.Task();
      resetWDT();
      currentUSBstate = usb.getUsbTaskState();
      if (lastUSBstate != currentUSBstate) {
        logCallback("USB state changed\r\n");
        switch (currentUSBstate) {
          case USB_ATTACHED_SUBSTATE_SETTLE: logCallback("Device Attached\r\n"); break;
          case USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE: logCallback("Detached, waiting for Device\r\n"); break;
          case USB_ATTACHED_SUBSTATE_RESET_DEVICE: logCallback("Resetting Device\r\n"); break;
          case USB_ATTACHED_SUBSTATE_WAIT_RESET_COMPLETE: logCallback("Reset complete\r\n"); break;
          case USB_STATE_CONFIGURING: logCallback("USB Configuring\r\n"); break;
          case USB_STATE_RUNNING: logCallback("USB Running\r\n"); setupWDT(9); break;
        }
        lastUSBstate = currentUSBstate;
      }
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
