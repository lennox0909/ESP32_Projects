/**
   HC-SR04 Demo
   Demonstration of the HC-SR04 Ultrasonic Sensor
   Date: August 3, 2016
   Description:
    Connect the ultrasonic sensor to the Arduino as per the
    hardware connections below. Run the sketch and open a serial
    monitor. The distance read from the sensor will be displayed
    in centimeters and inches.
   Hardware Connections:
    Arduino | HC-SR04
    -------------------
      5V    |   VCC
      7     |   Trig
      8     |   Echo
      GND   |   GND
   License:
    Public Domain
  
  LiquidCrystal_I2C 2004 with LED Back Light
  https:/github.com/johnrickman/LiquidCrystal_I2C
*/
#include <LiquidCrystal_I2C.h>


// Pins
const int TRIG_PIN = 2;
const int ECHO_PIN = 4;

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void setup() {
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  //Set Echo pin as input to measure the duration of
  //pulses coming back from the distance sensor
  pinMode(ECHO_PIN, INPUT);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(115200);
}

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  //float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  //inches = pulse_width / 148.0;

  // set cursor to first column, first row
  lcd.setCursor(0, 1);
  lcd.print(cm);
  delay(100);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Distance Detector");
  //lcd.setCursor(7, 1);
  //lcd.print("cm");
  
  // print message
  
  lcd.setCursor(7, 1);
  lcd.print("cm");
  //delay(100);
  //lcd.clear();

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
    //lcd.setCursor(3, 3);
    //lcd.print("Out of range");
    //lcd.clear();
  } else {
    Serial.print(cm);
    Serial.println(" cm \t");
    //Serial.print(inches);
    //Serial.println(" in");
  }

  // Wait at least 100ms before next measurement
  //delay(100);
}






