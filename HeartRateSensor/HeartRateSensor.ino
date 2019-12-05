/******************************************************************************
Heart_Rate_Display.ino
Demo Program for AD8232 Heart Rate sensor.
Casey Kuhns @ SparkFun Electronics
6/27/2014
https://github.com/sparkfun/AD8232_Heart_Rate_Monitor
The AD8232 Heart Rate sensor is a low cost EKG/ECG sensor.  This example shows
how to create an ECG with real time display.  The display is using Processing.
This sketch is based heavily on the Graphing Tutorial provided in the Arduino
IDE. http://www.arduino.cc/en/Tutorial/Graph
Resources:
This program requires a Processing sketch to view the data in real time.
Development environment specifics:
  IDE: Arduino 1.0.5
  Hardware Platform: Arduino Pro 3.3V/8MHz
  AD8232 Heart Monitor Version: 1.0
This code is beerware. If you see me (or any other SparkFun employee) at the
local pub, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.
******************************************************************************/

#include <LedControl.h>
#include <Wire.h>
#include "ThingSpeak.h"
#include <SPI.h>
#include <WiFi101.h>

const int DIN_PIN = 12;
const int CS_PIN = 11;
const int CLK_PIN = 10;

int pirPin = 2;

WiFiClient client;
char ssid[] = "PetersPhone";        //  your network SSID (name)
char pass[] = "00000000";   //  the network 


// Add yourThingSpeak channel information here
unsigned long myChannelNumber = 894602;
const char * myWriteAPIKey = "WQNFQULQV4AEVJV7";

// Checks how long has passed since the latest ThingSpeak upload (we will do one every 30 seconds)
int latestUpload;

// If motion was detected in the last 30 seconds, change this to 1
int motionDetected = 0;


const uint64_t IMAGES[] = {
  0x0000000000000000,
  0x0000808080800000,
  0x00004040c0400000,
  0x000020a060200000,
  0x000090d0b0900000,
  0x0000486858480000,
  0x0000a4b4aca40000,
  0x0000525a56520000,
  0x000029adab290000,
  0x0000945655940000,
  0x0000caab2aca0000,
  0x0000655515650000,
  0x000032aa8a320000,
  0x0000995545990000,
  0x0000ccaa22cc0000,
  0x0000665511660000,
  0x0000b3aa88330000,
  0x000059d544990000,
  0x00002cea22cc0000,
  0x000096f591660000,
  0x00004b7a48330000,
  0x0000253d24190000,
  0x0000121e120c0000,
  0x0000090f09060000,
  0x0000040704030000,
  0x0000020302010000,
  0x0000010101000000,
  0x0000000000000000
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
  
  // Initialise the ThingSpeak library
  ThingSpeak.begin(client);
  
  // Connects to the WiFi
  WiFi.begin(ssid, pass);
  delay(15000);   // Waits 15 seconds to confirm connection and finish calibrating the motion sensor
  
  // Print WiFi strength information
  printCurrentNet();
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
        
      if((digitalRead(8) == 1)||(digitalRead(9) == 1)){
    Serial.println('!');
      }
      else
      {
          //send the value of analog input 0:
          Serial.println(analogRead(A0));

              displayImage(IMAGES[i]);
              if (++i >= IMAGES_LEN ) {
                i = 0;
              }
    
        if(analogRead(A0) < 350) {
          tone(14, analogRead(A0), 50);
          
        }

            // Write the value of 'motionDetected' to ThingSpeak (0 or 1)
            ThingSpeak.setField(1, motionDetected);
            ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
       }
    
  //Wait for a bit to keep serial data from saturating
  delay(60);
  }
}

void printCurrentNet() {
  // Print the WiFi signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
