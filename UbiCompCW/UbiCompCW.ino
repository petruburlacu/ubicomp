#include <LedControl.h>
const int DIN_PIN = 12;
const int CS_PIN = 11;
const int CLK_PIN = 10;
int pirPin = 2;

const uint64_t IMAGES[] = {
  0x0000000000000000,
  0x0000000100000000,
  0x0000000300000000,
  0x0000040300000000,
  0x0008040300000000,
  0x00080c0300000000,
  0x00080c0b00000000,
  0x00080c0b08000000,
  0x00080c0b08080000,
  0x00080c0b08080800,
  0x00080c0b08180800,
  0x00080c0b28180800,
  0x00080c4b28180800,
  0x00080ccb28180800
};
const int IMAGES_LEN = sizeof(IMAGES)/8;

LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN);
void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(8, INPUT); // Setup for leads off detection LO +
  pinMode(9, INPUT); // Setup for leads off detection LO -
  display.clearDisplay(0);
  display.shutdown(0, false);
  display.setIntensity(0, 10);
  // In this example we have connected the
  // PIR Motion Sensor to digital pin 2
  pinMode(pirPin, INPUT_PULLUP);
}
void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(row, j));
    }
  }
}
int i = 0;
void loop() {
  int pirValue = digitalRead(pirPin);
    if (pirValue == LOW)
  {
     //If the value in digital pin 2 is LOW
     //something triggered the motion sensor
    Serial.println("Motion Detected");
    
      if((digitalRead(8) == 1)||(digitalRead(9) == 1)){
    Serial.println('!');
  }
  else{
     //send the value of analog input 0:
      Serial.println(analogRead(A0));
    }
    displayImage(IMAGES[i]);
    if (++i >= IMAGES_LEN ) {
      i = 0;
    }
    if(analogRead(A0) > 1) {
      tone(14, analogRead(A0), 50);
    }
    
  //Wait for a bit to keep serial data from saturating
  delay(50);
  }

}
