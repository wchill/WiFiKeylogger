// Require keyboard control library
#include <KeyboardController.h>

class KeyboardHandler {
  private:
    USBHost usb;
    KeyboardController keyboard;
    void (*logCallback)(char *output);
    void debug(char *output) {
      if(logCallback) {
        logCallback(output);
      }
    }
    void debugHex(int output) {
      if(logCallback) {
        char hex[5];
        sprintf(hex, "%x", output);
        logCallback(hex);
      }
    }
    void debugChar(char output) {
      if(logCallback) {
        char buf[] = {output, 0};
        logCallback(buf);
      }
    }
    uint32_t currentUSBstate;
    uint32_t lastUSBstate;
  public:
    KeyboardHandler() : keyboard(usb) {}
    bool init() {
      int ret = usb.Init() != -1;
      if(!ret) debug("USB Host did not start.\n");
      debug("USB Host started\n");
    }
    void task() {
      // Process USB tasks
      usb.Task();
    
      currentUSBstate = usb.getUsbTaskState();
      if (lastUSBstate != currentUSBstate) {
        debug("USB state changed: 0x");
        debugHex(lastUSBstate);
        debug(" -> 0x");
        debugHex(currentUSBstate);
        debug("\n");
        switch (currentUSBstate) {
          case USB_ATTACHED_SUBSTATE_SETTLE: debug("Device Attached\n"); break;
          case USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE: debug("Detached, waiting for Device\n"); break;
          case USB_ATTACHED_SUBSTATE_RESET_DEVICE: debug("Resetting Device\n"); break;
          case USB_ATTACHED_SUBSTATE_WAIT_RESET_COMPLETE: debug("Reset complete\n"); break;
          case USB_STATE_CONFIGURING: debug("USB Configuring\n"); break;
          case USB_STATE_RUNNING: debug("USB Running\n"); break;
        }
        lastUSBstate = currentUSBstate;
      }
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
    void pressed() {
      debugChar((char)(getKey() & 0xff));
    }
    void released() {
      //debug("Released key\n");
    }
    void setLogCallback(void (*callback)(char*)) {
      logCallback = callback;
    } 
};

/*
// This function intercepts key press
void keyPressed() {
  if(pressCallback) (*pressCallback)(keyboard.getOemKey(), keyboard.getModifiers(), keyboard.getKey());
}

// This function intercepts key release
void keyReleased() {
  if(releaseCallback) (*releaseCallback)(keyboard.getOemKey(), keyboard.getModifiers(), keyboard.getKey());
}

void printKey() {
  // getOemKey() returns the OEM-code associated with the key
  SerialDebug.print(" key:");
  SerialDebug.print(keyboard.getOemKey());

  // getModifiers() returns a bits field with the modifiers-keys
  int mod = keyboard.getModifiers();
  SerialDebug.print(" mod:");
  SerialDebug.print(mod);

  SerialDebug.print(" => ");

  if (mod & LeftCtrl)
    SerialDebug.print("L-Ctrl ");
  if (mod & LeftShift)
    SerialDebug.print("L-Shift ");
  if (mod & Alt)
    SerialDebug.print("Alt ");
  if (mod & LeftCmd)
    SerialDebug.print("L-Cmd ");
  if (mod & RightCtrl)
    SerialDebug.print("R-Ctrl ");
  if (mod & RightShift)
    SerialDebug.print("R-Shift ");
  if (mod & AltGr)
    SerialDebug.print("AltGr ");
  if (mod & RightCmd)
    SerialDebug.print("R-Cmd ");

  // getKey() returns the ASCII translation of OEM key
  // combined with modifiers.
  SerialDebug.write(keyboard.getKey());
  SerialDebug.println();
}
*/
