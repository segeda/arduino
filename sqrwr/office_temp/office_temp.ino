//#include <HIDSerial.h>

#define temp_sensor A1
#define red   8
#define green 12
#define blue  13

float min_temp = 20.0;
float max_temp = 27.0;

//HIDSerial serial;

void setup() {
  //serial.begin();

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

void loop() {
  int sens = analogRead(temp_sensor);
  float temp = (float)sens;
  temp = (temp * 3.25 / 1024 - 0.5) / 0.01;

  //serial.println(temp);

  if (temp < min_temp) {
    digitalWrite(blue, HIGH);
    delay(100);
    //serial.println("BLUE");
  }
  if (temp >= min_temp && temp <= max_temp) {
    digitalWrite(green, HIGH);
    delay(100);
    //serial.println("GREEN");
  }
  if (temp > max_temp) {
    digitalWrite(red, HIGH);
    delay(100);
    //serial.println("RED");
  }

  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  
  //serial.poll();

  delay(3000);
}


