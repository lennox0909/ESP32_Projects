// Wemos_LOLIN32


#include <U8g2lib.h>  // for OLED
#include <RCSwitch.h>
#include "hc_sr04.h"
#include "rgb_hue.h"

#define LED_BUILTIN 5

/* RCSwitch
  void enableTransmit(int nTransmitterPin);
  void disableTransmit();
  void setPulseLength(int nPulseLength);

  void setProtocol(Protocol protocol);
  void setProtocol(int nProtocol);
  void setProtocol(int nProtocol, int nPulseLength);

  void sendTriState(const char* sCodeWord);
  void send(unsigned long code, unsigned int length);
  void send(const char* sCodeWord);

  #if not defined( RCSwitchDisableReceiving )
    void enableReceive(int interrupt);
    void enableReceive();
    void disableReceive();
    bool available();
    void resetAvailable();
    unsigned long getReceivedValue();
    unsigned int getReceivedBitlength();
    unsigned int getReceivedDelay();
    unsigned int getReceivedProtocol();
    unsigned int* getReceivedRawdata();
  #endif

  void enableTransmit(int nTransmitterPin);
  void disableTransmit();
  void setPulseLength(int nPulseLength);
  void setRepeatTransmit(int nRepeatTransmit);
  #if not defined( RCSwitchDisableReceiving )
    void setReceiveTolerance(int nPercent);
  #endif
*/


/* --- Manual Enable Functions --- */
volatile bool OLED_display = 1;
/* --- Manual Enable Functions --- */

// Distance (cm) from Ultrasonic Sensor
volatile float cm;
volatile float triggerCM = 12.0;

// Messages on OLED
String oled_msg1;
String oled_msg2;
String oled_msg3;

// Touch Buttom
const uint8_t touchPin = 27;
volatile int touchThreshold = 40;
volatile int touchValue;

// Status info
volatile bool Relay_status = 0; // 0 = close status
volatile bool PAUSE_Mode = 0; // 0 = off

// OLED Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
// SCL: 22
// SDA: 21

// RCSwitch init

RCSwitch mySwitch = RCSwitch();
const uint8_t switch_transmit_pin = 13;
//const uint8_t switch_receiver_pin = 2;

// HCSR04 sensor
const uint8_t trig_pin = 32;
const uint8_t echo_pin = 33;
HCSR04 sonic(trig_pin, echo_pin);



// Define Tasks
TaskHandle_t handle_HueLED;
uint16_t stackSizeHueLED = 2300;
void Task_HueLED( void *pvParameters );
//void Task_Sonic( void *pvParameters );



void setup() {
  Serial.begin(115200);
  Serial.printf("堆積區大小：%u位元組\n", ESP.getHeapSize()); //堆積區大小：366824位元組
  Serial.printf("建立任務前的堆積：%u位元組\n", ESP.getFreeHeap()); //建立任務前的堆積：338912位元組
  xTaskCreate(
    Task_HueLED
    ,  "Hue LED" // A name just for humans
    ,  stackSizeHueLED        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    , NULL
    //,  (void*) &blink_delay // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,  1  // Priority
    ,  &handle_HueLED // Task handle is not used here - simply pass NULL
  );
  Serial.printf("建立任務後的堆積：%u位元組\n", ESP.getFreeHeap());
  pinMode (LED_BUILTIN, OUTPUT); // define LED as output interface
  digitalWrite (LED_BUILTIN, HIGH); // turn OFF LED


  // Transmitter is connected to Pin
  mySwitch.enableTransmit(switch_transmit_pin); // void enableTransmit(int nTransmitterPin);
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(1);
  // Optional set pulse length.
  mySwitch.setPulseLength(180);
  // Receiver is connected to Pin
  //mySwitch.enableReceive(switch_receiver_pin);


  if (OLED_display) {
    u8g2.begin();
    //  u8g2.setFont(u8g2_font_helvB14_tf);
    u8g2.setFont(u8g2_font_bytesize_tf);
  }
}

void loop() {
  UBaseType_t m = stackSizeHueLED - uxTaskGetStackHighWaterMark(handle_HueLED);
  Serial.printf("任務Hue_LED佔用 %u位元組\n", m);

//  if (mySwitch.available()) {
//    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(), mySwitch.getReceivedProtocol());
//    mySwitch.resetAvailable();
//  }

  touchValue = touchRead(touchPin);
  if (touchValue < touchThreshold) {
    if (PAUSE_Mode == 0) {
      PAUSE_Mode = 1;
      digitalWrite (LED_BUILTIN, LOW); // turn ON LED
      delay(1000);

    } else if (PAUSE_Mode == 1) {
      digitalWrite (LED_BUILTIN, HIGH); // turn OFF LED
      PAUSE_Mode = 0;
      delay(1000);
    }
  }


  if (PAUSE_Mode == 0) {
    cm = sonic.getCM(); //float cm;
    triggerRF433();

  } else if (PAUSE_Mode == 1) {

    cm = 400;
  }

  if (OLED_display) {
    if (Relay_status == 1) {
      oled_msg1 = "SW ON; T: " + (String)(triggerCM);
    } else {
      oled_msg1 = "SW OFF; T: " + (String)(triggerCM);
    }
    oled_msg2 = "At: " + (String)(cm) + " cm";
    oled_msg3 = "Touch: " + (String)(touchValue) + "; T: " + (String)(touchThreshold);

    if (PAUSE_Mode == 0) {
      if (OLED_display) {
        u8g2.clearBuffer();
        u8g2.drawStr(2, 14, oled_msg1.c_str());
        u8g2.drawStr(2, 38, oled_msg2.c_str());
        u8g2.drawStr(2, 62, oled_msg3.c_str());
        u8g2.sendBuffer();
      }
      Serial.print("Touch Value: ");
      Serial.print(touchValue);
      Serial.print("\t\t");
      Serial.print(cm);
      Serial.println(" cm");

    } else if (PAUSE_Mode == 1) {
      if (OLED_display) {
        oled_msg2 = ">>>>  PAUSE  <<<<";
        u8g2.clearBuffer();
        u8g2.drawStr(2, 14, oled_msg1.c_str());
        u8g2.drawStr(2, 38, oled_msg2.c_str());
        u8g2.drawStr(2, 62, oled_msg3.c_str());
        u8g2.sendBuffer();
      }
      Serial.println(">>>PAUSE on Distance Sensor & Power Switch<<<");
    }
  }

  // Wait a moment before next measurement
  delay(1000);
  //vTaskDelay(pdMS_TO_TICKS(500));
}

void triggerRF433() {
  if (cm < triggerCM) {
    if (Relay_status == 0) {
      /* Uusing decimal code */
      mySwitch.send(286955, 24); // 1_ON
      //mySwitch.send(283955, 24); // 1_ON
      //mySwitch.send(284099, 24); // 2_ON
      Relay_status = 1;
    } else if (Relay_status == 1) {
      mySwitch.send(286974, 24); // 1_OFF
      //mySwitch.send(283964, 24); // 1_OFF
      //mySwitch.send(284108, 24); // 2_OFF
      Relay_status = 0;
    }
  }
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
void Task_HueLED( void *pvParameters ) {
  (void) pvParameters;
  // RGB_HUE
  uint8_t ledR_pin = 19;
  uint8_t ledG_pin = 23;
  uint8_t ledB_pin = 18;
  RGB_HUE hue_led(ledR_pin, ledG_pin, ledB_pin);
  while (1) {
    hue_led.breathingHue();
    //UBaseType_t m = stackSizeHueLED - uxTaskGetStackHighWaterMark(handle_HueLED);
    //    UBaseType_t m = stackSizeHueLED - uxTaskGetStackHighWaterMark(NULL);
    //    Serial.printf("任務Hue_LED佔用 %u位元組\n", m);
  }
}
