#include <LedControl.h>
#include <Wire.h>
#include "ThingSpeak.h"
#include <SPI.h>
#include <WiFi101.h>

WiFiClient client;
char ssid[] = "PetersPhone";        //  your network SSID (name)
char pass[] = "00000000";   //  the network password


// Add yourThingSpeak channel information here
unsigned long myChannelNumber = 928770;
const char * myWriteAPIKey = "AXU7PDC2DAXHAZSX";


const int DIN_PIN = 12;
const int CS_PIN = 11;
const int CLK_PIN = 10;
int pirPin = 2;
int motionDetected = 0;

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

  // Initialise the ThingSpeak library
  ThingSpeak.begin(client);
  
  // Connects to the WiFi
  WiFi.begin(ssid, pass);
  delay(15000);   // Waits 15 seconds to confirm connection and finish calibrating the motion sensor
  
  // Print WiFi strength information
  printCurrentNet();
  
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
    motionDetected ++;
   int frequency = analogRead(A0);

    // Write the value of 'motionDetected' to ThingSpeak (0 or 1)
    ThingSpeak.setField(1, motionDetected);
    ThingSpeak.setField(2, frequency);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
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

void printCurrentNet() {
  // Print the WiFi signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
