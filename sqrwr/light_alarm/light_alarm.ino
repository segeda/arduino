//#include <HIDSerial.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "pitches.h"

#define buzzer 9
#define red   8
#define blue  13
#define light_sensor A0

//HIDSerial serial;

static long duration = 500;
int ton = NOTE_A4;
int barva = 8;

volatile int f_wdt = 1;

ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  /* Now enter sleep mode. */
  sleep_mode();  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */

  /* Re-enable the peripherals. */
  power_all_enable();
}

void setup() {
  //serial.begin();
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(blue, LOW);

  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  /* set new watchdog timeout prescaler value */
   WDTCSR = 1<<WDP1 | 1<<WDP2;            // 1.0 seconds
  /* other options (see page 57 of ATmega328 datasheet)
   WDTCSR = 1<<WDP2;                      // 0.25 seconds
   WDTCSR = 1<<WDP0 | 1<<WDP2;            // 0.5 seconds
   WDTCSR = 1<<WDP1 | 1<<WDP2;            // 1.0 seconds
   WDTCSR = 1<<WDP0 | 1<<WDP1 | 1<<WDP2;  // 2.0 seconds
   WDTCSR = 1<<WDP3;                      // 4.0 seconds
   WDTCSR = 1<<WDP3 | 1<<WDP0;            // 8.0 seconds
   */

  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
}

void loop() {
  int light_value;
  if(f_wdt==1) {
    light_value = analogRead(light_sensor);
    if (light_value > 400) {
      if (ton == NOTE_A4) {
        ton = NOTE_C4;
        barva = 13;
      } 
      else {
        ton = NOTE_A4;
        barva = 8;
      }
      //serial.println(light_value);
      //serial.poll();
      tone(buzzer, ton, duration);
      digitalWrite(barva, HIGH);
      delay(duration);
      digitalWrite(barva, LOW);
    } 
    else {
      f_wdt = 0;
      enterSleep();
    }
  }
}




