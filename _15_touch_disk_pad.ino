#include <Wire.h>
#define ttp229 (0xAF>>1) //If you have a datasheet or sample code that uses 8 bit address, 
                         //you'll want to drop the low bit (i.e. shift the value one bit to the right), 
                         //yielding an address between 0 and 127.
#define maxKeyNumber 15
int lastPressedKeys[maxKeyNumber];
int currentPressedKeys[maxKeyNumber];

void setup() {
  Serial.begin(9600);  // start serial for output
  Wire.begin();
}

void loop() {
  delay(500);
  bool isNewData = false;
  Wire.requestFrom(ttp229,2,true);
  while (Wire.available()) { 
    uint16_t b1 = Wire.read(); // receive a first byte
    uint16_t b2 = Wire.read(); // receive a second byte
    if (b1==b2 && b2==0) {break;}
    isNewData = true;
    int keys = (b1<<8) + b2;
    for(int i=0; i<maxKeyNumber; i++) {
      if (bitRead(keys, maxKeyNumber-i) == 1){
        lastPressedKeys[i] = 1;
      }
      else {
        lastPressedKeys[i] = 0;
      }
    }
  }
  if (isNewData) {
    Serial.print("Pressed keys:");
    for(int i=0; i<maxKeyNumber; i++) {
      if (lastPressedKeys[i] == 1) {
        Serial.print(" ");
        Serial.print(i+1);
        Serial.print(";");
      }
    }
    Serial.println();
    Serial.println("---------------");
    isNewData = false;
  }
}


