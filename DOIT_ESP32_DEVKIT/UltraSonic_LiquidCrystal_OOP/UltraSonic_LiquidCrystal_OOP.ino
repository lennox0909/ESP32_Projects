
#include "hc_sr04.h"
#include <LiquidCrystal_I2C.h>

// DOIT_ESP32_DEVKIT

// Set sensor pins
HCSR04 sensor1(2, 4);
HCSR04 sensor2(26, 25);

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);



void setup() {
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

void loop() {


  float cm1;
  float cm2;

  cm1 = sensor1.getCM();
  cm2 = sensor2.getCM();

  lcd.setCursor(0, 1);
  lcd.print((String)(cm1));
  lcd.setCursor(0, 2);
  lcd.print(cm2);
  //delay(500);
  //lcd.clear();

  lcd.setCursor(0, 0);  // set cursor to first column, first row
  lcd.print("Distance Detector");


  lcd.setCursor(9, 1);
  lcd.print("cm Sensor01");
  lcd.setCursor(9, 2);
  lcd.print("cm Sensor02");


  Serial.print(cm1);
  Serial.println(" cm Sensor01 \t");
  Serial.print(cm2);
  Serial.println(" cm Sensor01 \t");
  Serial.println("");


  if (cm2 < 10.0) {
    lcd.noBacklight();
  } else {
    lcd.backlight();
  }

  // Wait at least 100ms before next measurement
  delay(100);
}
