/**
   HC-SR04 Demo
   Demonstration of the HC-SR04 Ultrasonic Sensor
   Description:
    Connect the ultrasonic sensor to the Arduino as per the
    hardware connections below. Run the sketch and open a serial
    monitor. The distance read from the sensor will be displayed
    in centimeters and inches.

   License:
    Public Domain
*/

class HCSR04 {

private:
  // Anything over 400 cm (23200 us pulse) is "out of range"
  unsigned int _MAX_DIST = 23200;  
  unsigned long _t1;               
  unsigned long _t2;               
  unsigned long _pulse_width;      
  float _cm;                       

public:
  // Pins
  int TRIG_PIN;
  int ECHO_PIN;

  HCSR04(int trig, int echo) {
    TRIG_PIN = trig;
    ECHO_PIN = echo;

    // The Trigger pin will tell the sensor to range find
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);
    
    //Set Echo pin as input to measure the duration of
    //pulses coming back from the distance sensor
    pinMode(ECHO_PIN, INPUT);
  }

  float getCM() {
    // Hold the trigger pin high for at least 10 us
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Wait for pulse on echo pin
    while (digitalRead(ECHO_PIN) == 0)
      ;
    // Measure how long the echo pin was held high (pulse width)
    // Note: the micros() counter will overflow after ~70 min
    _t1 = micros();
    while (digitalRead(ECHO_PIN) == 1)
      ;
    _t2 = micros();
    _pulse_width = _t2 - _t1;

    // Calculate distance in centimeters and inches. The constants
    // are found in the datasheet, and calculated from the assumed speed
    //of sound in air at sea level (~340 m/s).
    _cm = _pulse_width / 58.0;
    return _cm;
  }
};
