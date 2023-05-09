/*
  ledcWrite_RGB.ino
  Runs through the full 255 color spectrum for an rgb led
  Demonstrate ledcWrite functionality for driving leds with PWM on ESP32

  This example code is in the public domain.

  Some basic modifications were made by vseven, mostly commenting.
*/

/*
  Esp-S3-12K-Kit

  OnBoard RGB OLED pins: RGB = 5,6,7

  GPIO-08: SDA
  GPIO-09: SCL
*/

#include <LiquidCrystal_I2C.h>
#include "rgb_hue.h"

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Set up the rgb led names
RGB_HUE bulb(5, 6, 7);

// the setup routine runs once when you press reset:
void setup()
{
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  Serial.begin(115200);
  delay(10);

}

// void loop runs over and over again
void loop()
{
  lcd.setCursor(0, 0);  // set cursor to first column, first row
  lcd.print("RGB Tuner");
  lcd.setCursor(0, 1);
  lcd.print("RED:");
  lcd.setCursor(0, 2);
  lcd.print("GREEN:");
  lcd.setCursor(0, 3);
  lcd.print("BLUE:");




  //Serial.println("Send all LEDs a 255 and wait 2 seconds.");
  //bulb.allBright();
  //delay(2000);
  //  Serial.println("Send all LEDs a 0 and wait 2 seconds.");
  //  bulb.allDark();
  //  delay(1000);

  //Serial.println("Starting color fade loop.");
  //bulb.breathingHue();


}
