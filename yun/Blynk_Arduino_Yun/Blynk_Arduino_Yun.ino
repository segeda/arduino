#include <Process.h>
#include <Bridge.h>
#include <BridgeClient.h>

Process date;
int32_t whenTime;
int32_t lastTime = -1;
BridgeClient client;

#include <BlynkSimpleYun.h>

#define VPIN_HUMIDITY V1
#define VPIN_TEMP V2

char auth[] = "<AUTH-KEY>"; 

#include <SimpleTimer.h>

SimpleTimer timer;

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "<MQTT-SERVER>"
#define AIO_SERVERPORT  1883

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Publish when = Adafruit_MQTT_Publish(&mqtt, "arduino/yun/time");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, "arduino/yun/humidity");
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, "arduino/yun/temperature");
Adafruit_MQTT_Publish heatIndex = Adafruit_MQTT_Publish(&mqtt, "arduino/yun/heatIndex");

void setup()
{
  dht.begin();
  delay(10);
  Blynk.begin(auth);
  Bridge.begin();

  // run an initial date process. Should return:
  // seconds since 1970-01-01 00:00:00 UTC.
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%s");
    date.run();
  }

  // Setup a function to be called every second
  timer.setInterval(2000L, sendValues);
}

void sendValues()
{
  //Read the Temp and Humidity from DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    return;
  }

  if (lastTime != whenTime) { // if a second has passed
    when.publish(whenTime);

    // restart the date process:
    if (!date.running()) {
      date.begin("date");
      date.addParameter("+%s");
      date.run();
    }
  }
  
  float hic = dht.computeHeatIndex(t, h, false);
  Blynk.virtualWrite(1, (int)h);
  Blynk.virtualWrite(2, (int)t);
  Blynk.virtualWrite(3, hic);

  humidity.publish((int32_t)h);
  temperature.publish((int32_t)t);
  heatIndex.publish(hic);

  //if there's a result from the date process, parse it:
  while (date.available() > 0) {
    String timeString = date.readString();
    lastTime = whenTime;
    whenTime = timeString.toInt();
  }
}

void loop()
{
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  Blynk.run();
  timer.run();
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
}
