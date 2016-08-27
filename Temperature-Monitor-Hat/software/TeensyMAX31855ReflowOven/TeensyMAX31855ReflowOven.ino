/***************************************************

  Jenner Hanni / Wickerbox Electronics Notes

  I modified the code to just add the Time library
  and to display F and C over serial so I
  can monitor the temperature from my reflow oven
  using the MAX31855 with a Teensy 3.1.

  It includes Michael Margolis' Time library that provides a handy
  API for all the time-tracking. Super useful.
  http://www.pjrc.com/teensy/td_libs_Time.html

  ********************************************
  Original Adafruit notice:
  ********************************************

  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <Time.h>
#include "Adafruit_MAX31855.h"
#include <LiquidCrystal.h>

// Track the time using Margolis' time tracking library
time_t reftime = 0;
time_t nowtime = 0;
time_t prevmin = 0;

// Hook up to the thermocouple over SPI
#define DO   12
#define CS   14
#define CLK  13
Adafruit_MAX31855 thermocouple(CLK, CS, DO);

// LCD
// LiquidCrystal lcd(rs, rw, enable, d4, d5, d6, d7)
LiquidCrystal lcd(4, 5, 6, 8, 9, 10, 11);

// Button value
int val = 1;
int pinButton = 2;

// Speaker value
int pinSpeaker = 20;

void setup() {

  // set up serial port
  Serial.begin(9600);

  // set up the LCD number of (columns, rows)
  lcd.begin(16, 2);
  lcd.print("Reflow Monitor");

  // get the reference time
  reftime = now();
  nowtime = now() - reftime;

  Serial.println("MAX31855 Reflow Oven");
  // wait for MAX chip to stabilize
  delay(500);

  // set up the button time reset input
  // it will be active low
  pinMode(pinButton, INPUT_PULLUP);

  // test the speaker
  pinMode(pinSpeaker, OUTPUT);
  analogWrite(20,200);
  delay(2000);
  analogWrite(20,0);
}

void loop() {

  // reset the reference time upon a button press
  if (digitalRead(pinButton) == 0)
    reftime = now();

  // update number of seconds since startup
  nowtime = now() - reftime;

  // read from the thermocouple
  double c = thermocouple.readCelsius();
  Serial.println(c);

  // if the thermocouple returned a good value,
  // display the temp and time
  // otherwise, display the error message
  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
    lcd.setCursor(0, 0);
    lcd.print("Read Failed.");
  } else {
    double f = thermocouple.readFarenheit();
    Serial.println(thermocouple.readFarenheit());
    Serial.print(nowtime);
    Serial.print(" seconds   C = ");
    Serial.print(c);
    Serial.print("   F = ");
    Serial.print(f);
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.print(c);
    lcd.print("C  ");
    lcd.print(f);
    lcd.print("F");
    lcd.setCursor(0,1);
    lcd.print("Elapsed: ");
    lcd.print(nowtime);
  }

  delay(1000);
}
