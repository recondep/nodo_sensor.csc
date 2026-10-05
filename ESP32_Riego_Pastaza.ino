#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Red_Agricola_UEA";
const char* password = "Amazonia2026";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
const int pinSensor = 34;
const int pinRele = 25;

void callback(char* topic, byte* payload, unsigned int length) {
  String cmd = "";
  for (int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }
  Serial.print("Mensaje en [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(cmd);

  if (cmd == "1") {
    digitalWrite(pinRele, HIGH); // Activa riego
    Serial.println("Riego ACTIVADO");
  } else {
    digitalWrite(pinRele, LOW); // Desactiva riego
    Serial.println("Riego DESACTIVADO");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32_Sensor_Rosa")) {
      Serial.println("Conectado!");
      client.subscribe("uea/agro/control");
    } else {
      Serial.print("Error: ");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pinRele, OUTPUT);
  digitalWrite(pinRele, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  int
