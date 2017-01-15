#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <Bridge.h>
#include <BlynkSimpleYun.h>

#include <SimpleTimer.h>

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 2
#define DHTTYPE DHT11

char auth[] = "<AUTH-KEY>";

SimpleTimer timer;

DHT dht(DHTPIN, DHTTYPE);

#define VPIN_HUMIDITY V1
#define VPIN_TEMP V2

void setup()
{
  Serial.begin(9600);
  dht.begin();
  delay(10);
  Blynk.begin(auth);
  // Setup a function to be called every second
  timer.setInterval(10000L, sendUptime);
}

void sendUptime()
{
  //Read the Temp and Humidity from DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    return;
  }
  
  float hic = dht.computeHeatIndex(t, h, false);
  Blynk.virtualWrite(1, h);
  Blynk.virtualWrite(2, t);
  Blynk.virtualWrite(3, hic);
}

void loop()
{
  Blynk.run();
  timer.run();
}

