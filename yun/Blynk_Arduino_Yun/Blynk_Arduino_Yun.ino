#include <Bridge.h>
#include <BridgeClient.h>
#include <PubSubClient.h>
#include <BlynkSimpleYun.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define VPIN_HUMIDITY V1
#define VPIN_TEMP V2
#define DHTPIN 2
#define DHTTYPE DHT11
#define ROOT_TOPIC "arduino/yun"

char auth[] = "<BLYNK-AUTH-KEY>";
SimpleTimer timer;
DHT dht(DHTPIN, DHTTYPE);
BridgeClient client;
PubSubClient pubSubClient(client);

void setup()
{
  dht.begin();
  delay(10);
  Blynk.begin(auth);
  Bridge.begin();

  pubSubClient.setServer("<MQTT-SERVER>", 1883);

  timer.setInterval(10 * 1000, sendValues); //every 10 seconds
}

void sendValues()
{
  //Read the Temp and Humidity from DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);
  Blynk.virtualWrite(1, (int)h);
  Blynk.virtualWrite(2, (int)t);
  Blynk.virtualWrite(3, hic);

  String payload = "{\"humidity\":";
  payload += (int)h;
  payload += ",\"temperature\":";
  payload += (int)t;
  payload += ",\"heatIndex\":";
  payload += hic;
  payload += "}";
  int payload_length = payload.length() + 1;
  char payload_data[payload_length];
  payload.toCharArray(payload_data, payload_length);
  pubSubClient.publish(ROOT_TOPIC, payload_data);

  String humidity = String((int)h);
  int humidity_length = humidity.length() + 1;
  char humidity_data[humidity_length];
  humidity.toCharArray(humidity_data, humidity_length);
  pubSubClient.publish(ROOT_TOPIC "/humidity", humidity_data);

  String temperature = String((int)t);
  int temperature_length = temperature.length() + 1;
  char temperature_data[temperature_length];
  temperature.toCharArray(temperature_data, temperature_length);
  pubSubClient.publish(ROOT_TOPIC "/temperature", temperature_data);

  String heatIndex = String(hic);
  int heatIndex_length = heatIndex.length() + 1;
  char heatIndex_data[heatIndex_length];
  heatIndex.toCharArray(heatIndex_data, heatIndex_length);
  pubSubClient.publish(ROOT_TOPIC "/heatIndex", heatIndex_data);
}

void loop()
{
  if (!pubSubClient.connected()) {
    reconnect();
  }
  pubSubClient.loop();

  Blynk.run();
  timer.run();
}

void reconnect() {
  // Loop until we're reconnected
  while (!pubSubClient.connected()) {
    // Attempt to connect
    if (!pubSubClient.connect("arduinoYun")) {
      // Wait 5 seconds before retrying
      delay(5 * 1000);
    }
  }
}
