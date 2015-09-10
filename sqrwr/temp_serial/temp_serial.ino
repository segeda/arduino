#include <HIDSerial.h>

#define temp_sensor A1

HIDSerial serial;

void setup() {
  serial.begin();
}

void loop() {
  int sens = analogRead(temp_sensor);
  float temp = (float)sens;
  temp = (temp * 3.25 / 1024 - 0.5) / 0.01;
  
  serial.print(sens);
  serial.print(" | ");
  serial.println(temp);
  serial.poll();
  
  delay(500);
}
