#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Bridge.begin();
  digitalWrite(LED_BUILTIN, LOW);

  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  YunClient client = server.accept();
  if (client) {
    String command = client.readString();
    command.trim();
    if (command == "read") {
      int value = analogRead(A0);
      client.print(value);
    }
    client.stop();
  }

  delay(50);
}
