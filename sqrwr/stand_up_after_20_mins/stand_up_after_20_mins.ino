// *********************************************
//
// 20 mins lights GREEN led, after them RED led
// if push BUTTON, interval is reset
//
// *********************************************

//#include <HIDSerial.h>

#define BUTTON  4
#define GREEN  12
#define RED     8

//HIDSerial serial;

const long interval = 1200000; // 20 * 60 * 1000 = 20 min
unsigned long goTime;

void setup() {
  //serial.begin();
  
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, HIGH);

  pinMode(RED, OUTPUT);
  digitalWrite(RED, LOW);
  
  goTime = millis() + interval;
}

void loop() {
  unsigned long currentTime = millis();
  
  //serial.print(currentTime);
  //serial.print(">=");
  //serial.println(goTime);
  //serial.poll();

  if(digitalRead(BUTTON) == LOW) {
    goTime = currentTime + interval;
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
  } 
  else {
    if (currentTime >= goTime) {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
    } 
    else {
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
    }
  }
  delay(5);
}



