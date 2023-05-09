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
    unsigned int MAX_DIST = 23200;
    unsigned long t1;
    unsigned long t2;
    unsigned long pulse_width;
    bool pin_status;
    float cm;

  public:
    // Pins
    uint8_t TRIG_PIN;
    uint8_t ECHO_PIN;
    uint8_t VCC_PIN;

    HCSR04(uint8_t trig, uint8_t echo, uint8_t vcc_pin) {
      TRIG_PIN = trig;
      ECHO_PIN = echo;
      VCC_PIN = vcc_pin;
      enablePins();
    }

    void enablePins() {
      // The Trigger pin will tell the sensor to range find
      pinMode(TRIG_PIN, OUTPUT);
      digitalWrite(TRIG_PIN, LOW);

      //Set Echo pin as input to measure the duration of
      //pulses coming back from the distance sensor
      pinMode(ECHO_PIN, INPUT);

      // Power the sensor
      pinMode(VCC_PIN, OUTPUT);
      digitalWrite(VCC_PIN, HIGH);

      pin_status = 1;
    }

    void disablePins() {
      // The Trigger pin will tell the sensor to range find
      pinMode(TRIG_PIN, INPUT);

      //Set Echo pin as input to measure the duration of
      //pulses coming back from the distance sensor
      pinMode(ECHO_PIN, INPUT);

      // Power the sensor
      pinMode(VCC_PIN, INPUT);

      pin_status = 0;
    }

    float getCM() {
      
      if ( pin_status == 0 ) {
        return 400.0;
      }
      
      // Hold the trigger pin high for at least 10 us
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);

      // Wait for pulse on echo pin
      while (digitalRead(ECHO_PIN) == 0)
        ;
      // Measure how long the echo pin was held high (pulse width)
      // Note: the micros() counter will overflow after ~70 min
      t1 = micros();
      while (digitalRead(ECHO_PIN) == 1)
        ;
      t2 = micros();
      pulse_width = t2 - t1;

      // Calculate distance in centimeters and inches. The constants
      // are found in the datasheet, and calculated from the assumed speed
      //of sound in air at sea level (~340 m/s).
      cm = pulse_width / 58.0;

      if ( cm > 400.0 ) {
        cm = 400.0;
      }
      return cm;
    }
};
