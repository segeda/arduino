// *********************************************
//
// This program count opened box
//
// *********************************************

#include <HIDSerial.h>

#define BUTTON 4
#define light_sensor A0
#define green 12


#define HIDSERIAL_DEBUG
#ifdef HIDSERIAL_DEBUG  
  HIDSerial serial;
#endif

void setup() {
  #ifdef HIDSERIAL_DEBUG
    serial.begin();
  #endif
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);

  pinMode(green, OUTPUT);
  digitalWrite(green, LOW);
}

void loop() {
  int light_value;
  light_value = analogRead(light_sensor);

  #ifdef HIDSERIAL_DEBUG
      serial.println(light_value);
      serial.poll();
  #endif

  digitalWrite(green, HIGH);
  delay(100);
  digitalWrite(green, LOW);
  
  delay(1000);
}
