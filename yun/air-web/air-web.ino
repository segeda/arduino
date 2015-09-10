#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

YunServer server;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Bridge.begin();
  digitalWrite(LED_BUILTIN, LOW);

  server.listenOnLocalhost();
  server.begin();

  dht.begin();
}

void loop() {
  delay(2000);

  YunClient client = server.accept();
  if (client) {
    String command = client.readString();
    command.trim();
    if (command == "dht11") {
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      if (isnan(h) || isnan(t)) {
        client.print("Failed to read from DHT sensor!");
        return;
      }

      float hic = dht.computeHeatIndex(t, h, false);

      client.print("Humidity: ");
      client.print(h);
      client.print(" %\t");
      client.print("Temperature: ");
      client.print(t);
      client.print(" *C\t");
      client.print("Heat index: ");
      client.print(hic);
      client.print(" *C");
    }
    client.stop();
  }
}
