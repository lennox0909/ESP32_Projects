/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
const uint8_t relay_pin = 9;
unsigned long Val;

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(2);  // Receiver on interrupt 0 => that is pin #2
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);


}

void loop() {
  if (mySwitch.available()) {
    Val = mySwitch.getReceivedValue();
    Serial.print("Received ");
    //Serial.print( mySwitch.getReceivedValue() );
    Serial.println( Val );
    //Serial.print(" / ");
    //Serial.print( mySwitch.getReceivedBitlength() );
    //Serial.print("bit ");
    //Serial.print("Protocol: ");
    //Serial.println( mySwitch.getReceivedProtocol() );
    if ( Val == 286955 ) {
      digitalWrite(relay_pin, HIGH);
    } else if ( Val == 286974 ) {
      digitalWrite(relay_pin, LOW);
    }

    mySwitch.resetAvailable();
  }
}
