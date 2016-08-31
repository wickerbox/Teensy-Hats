/***************************************************

  Jenner Hanni / Wickerbox Electronics Notes
  See the Github repo: https://github.com/wickerbox/Teensy-Hats/

****************************************************/


#include <SPI.h>
#include "SdFat.h"
#include <LiquidCrystal.h>

// Hook up to the SD card over SPI
// Assumes 11 MOSI, 12 MISO, 13 SCLK
int SDCS_Pin = 10;
SdFat SD;

// LCD
// LiquidCrystal lcd(rs, rw, enable, d4, d5, d6, d7)
LiquidCrystal lcd(2, 3, 4, 6, 7, 8, 9);

// LEDs and Buttons
int LED1_Pin = 5;
int LED2_Pin = 14;
int Button1_Pin = 17;
int Button2_Pin = 16;
int Button3_Pin = 15;

// LiDAR Setup
int LiDAR_Trigger_Pin = 21;
int LiDAR_Monitor_Pin = 22;
int LiDAR_Enable_Pin = 20;
int LiDAR_SDA_Pin = 18;
int LiDAR_SCL_Pin = 19;
unsigned long pulse_width;

void readLiDAR() {
  pulse_width = pulseIn(LiDAR_Monitor_Pin, HIGH); // Count how long the pulse is high in microseconds
  if (pulse_width != 0) { // If we get a reading that isn't zero, let's print it
    pulse_width = pulse_width / 10; // 10usec = 1 cm of distance for LIDAR-Lite
  }
}

void setup() {

  Serial.begin(9600);
  delay(1000);
  Serial.println("The program is on.");
  delay(1000);

  // Setup the LiDAR Rangefinder
  pinMode(LiDAR_Trigger_Pin, OUTPUT);
  pinMode(LiDAR_Monitor_Pin, INPUT);
  pinMode(LiDAR_Enable_Pin, OUTPUT);
  digitalWrite(LiDAR_Enable_Pin, HIGH); // turn sensor on
  digitalWrite(LiDAR_Trigger_Pin, LOW); // set for continuous read

  // Setup the buttons and LEDs
  pinMode(LED1_Pin, OUTPUT);
  pinMode(LED2_Pin, OUTPUT);
  pinMode(Button1_Pin, INPUT);
  pinMode(Button2_Pin, INPUT);
  pinMode(Button2_Pin, INPUT);

  // LED Test
  digitalWrite(LED1_Pin, HIGH);
  digitalWrite(LED2_Pin, HIGH);
  delay(2000);
  digitalWrite(LED1_Pin, LOW);
  digitalWrite(LED2_Pin, LOW);

  // Setup the LCD
  lcd.begin(16, 2);
  lcd.print("LCD-LiDAR-SD Hat");
  
}

void loop() {
  // check buttons
  digitalWrite(LED1_Pin, not(digitalRead(Button1_Pin)));
  digitalWrite(LED2_Pin, not(digitalRead(Button2_Pin)));
  
  // check lidar
  readLiDAR();
  
  // update LCD
  lcd.setCursor(0, 1);
  lcd.print("Range: ");
  lcd.print(pulse_width);
}
