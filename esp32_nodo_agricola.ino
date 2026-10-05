#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Red_Agricola_UEA";
const char* password = "Amazonia2026";
const char* mqtt_server = "://hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
const int pinSensor = 34;
const int pinRele = 25;

void setup() {
  Serial.begin(115200);
  pinMode(pinRele, OUTPUT);
  digitalWrite(pinRele, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String cmd = "";
  for (int i = 0; i < length; i++) { cmd += (char)payload[i]; }
  if (cmd == "1") {
    digitalWrite(pinRele, HIGH); // Abre válvula de riego
    Serial.println("Riego ACTIVADO");
  } else {
    digitalWrite(pinRele, LOW);  // Cierra válvula de riego
    Serial.println("Riego DESACTIVADO");
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Sensor_Rosa")) {
      client.subscribe("uea/agro/control");
    } else { delay(5000); }
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  int lectura = analogRead(pinSensor);
  char msg[10];
  sprintf(msg, "%d", lectura);
  client.publish("uea/agro/humedad", msg);
  delay(10000); // Ventana de transmisión óptima por ráfagas
}
