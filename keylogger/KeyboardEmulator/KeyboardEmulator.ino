#define CMD_START     0xFF
#define CMD_PRESS     0x01
#define CMD_RELEASE   0x02
#define CMD_CHANGEMOD 0x04

uint8_t modifiers;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial1.begin(9600);
  Serial.begin(9600);
  Keyboard.begin();

  delay(1000);
  
  Serial.println("Hello world!");
  digitalWrite(13, LOW);
}

void loop() {
  //if(Serial1.available() > 0) {
  
    while(!Serial1.available());
    digitalWrite(13, HIGH);
    Serial.println("Reading");
    uint8_t command = Serial1.read();
    Serial.println("Data1");
    while(!Serial1.available());
    uint8_t d_high = Serial1.read();
    Serial.println("Data2");
    while(!Serial1.available());
    uint8_t d_low = Serial1.read();
    //uint16_t data = (d_high >> 8) | (d_low);
    Serial.println("Handling");
    //data = KEY_A;
    switch(command) {
      case CMD_CHANGEMOD:
        Serial.println("Modifiers changed");
        Keyboard.set_modifier(d_low);
        break;
      case CMD_PRESS:
        Serial.print("Pressed ");
        Serial.print(d_high, HEX);
        Serial.println(d_low, HEX);
        Keyboard.pressRaw(d_low);
        break;
      case CMD_RELEASE:
        Serial.print("Released ");
        Serial.print(d_high, HEX);
        Serial.println(d_low, HEX);
        Keyboard.releaseRaw(d_low);
        break;
      default:
        Serial.println("Unknown cmd");
        break;
    }
    
    Serial.flush();
    digitalWrite(13, LOW);
  //}
}

