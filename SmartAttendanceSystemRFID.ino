#include <WiFiNINA.h>
#include <PubSubClient.h>

const char* ssid = "WiFi-LabIoT";
const char* password = "s1jzsjkw5b";

const char* mqttServer = "demo.thingsboard.io";
const int mqttPort = 1883;
const char* accessToken = "n1tchLXokpcMfAulagL5"; // Replace with your ThingsBoard device token


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  connectWiFi();
  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to ThingsBoard...");
    if (client.connect("ArduinoUno", accessToken, NULL)) {
      Serial.println("Connected to ThingsBoard!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Send sample data
  String payload = "{\"attendance\":\"Present\", \"student_id\":\"12345\"}";
  client.publish("v1/devices/me/telemetry", payload.c_str());
  delay(5000); // Send data every 5 seconds
}

void connectWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  delay(2000);
  String cmd = "AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"";
  delay(5000);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting to ThingsBoard...");
    if (client.connect("ArduinoUno", accessToken, NULL)) {
      Serial.println("Reconnected to ThingsBoard!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
    }
  }
}