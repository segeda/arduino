#include <Bridge.h>
#include <HttpClient.h>
#include <Process.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define MQ2 A0
#define LDR A1
#define DUST A2
#define DUST_LED 3

DHT dht(DHTPIN, DHTTYPE);
Process date;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float h_min = 50.0;
float h_max = -50.0;
float t_min = 50.0;
float t_max = -50.0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Bridge.begin();
  dht.begin();
  pinMode(DUST_LED, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  delay(10000);

  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%s");
    date.run();
  }
  String timestamp;
  while (date.available() > 0) {
    timestamp = date.readString();
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    //Failed to read from DHT sensor!
    return;
  }

  h_min = min(h_min, h);
  h_max = max(h_max, h);
  t_min = min(t_min, t);
  t_max = max(t_max, t);

  float hic = dht.computeHeatIndex(t, h, false);

  float mq2value;
  int mq2sensor = analogRead(MQ2);
  mq2value = (float)mq2sensor / 1024 * 5.0;

  int ldrsensor = analogRead(LDR);

  digitalWrite(DUST_LED, LOW);
  delayMicroseconds(samplingTime);

  float voMeasured = analogRead(DUST);

  delayMicroseconds(deltaTime);
  digitalWrite(DUST_LED, HIGH);
  delayMicroseconds(sleepTime);

  float calcVoltage = voMeasured * (5.0 / 1024.0);
  float dustDensity = 0.17 * calcVoltage - 0.1;

  String url = "https://www.dweet.io/dweet/for/segeda-air-freeboard";
  url = url + "?temperature=";
  url = url + t;
  url = url + "&temperature_min=";
  url = url + t_min;
  url = url + "&temperature_max=";
  url = url + t_max;
  url = url + "&humidity=";
  url = url + h;
  url = url + "&humidity_min=";
  url = url + h_min;
  url = url + "&humidity_max=";
  url = url + h_max;
  url = url + "&heat_index=";
  url = url + hic;
  url = url + "&mq2=";
  url = url + mq2value;
  url = url + "&ldr=";
  url = url + ldrsensor;
  url = url + "&dust_raw=";
  url = url + voMeasured;
  url = url + "&dust_voltage=";
  url = url + calcVoltage;
  url = url + "&dust_density=";
  url = url + dustDensity;
  url = url + "&timestamp=";
  url = url + timestamp;

  HttpClient client;
  client.noCheckSSL();
  client.get(url);
  while (client.available()) {
    char c = client.read();
  }
}
