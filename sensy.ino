// Libraries

// Wifi
#include <WiFi.h>

// DHT
#include <DHT.h>
#include <DHT_U.h>
#include <DHTesp.h>
#include <Adafruit_Sensor.h>

// MQTT
#include <PubSubClient.h>
////////////////////////////////////


// Constantes

// Wifi
const char* ssid = "NOMBRE_WIFI";
const char* password = "PASSWORD_WIFI";

// MQTT
const char* mqtt_server_host = "172.17.0.1"; // Docker GATEWAY (default)
const uint16_t mqtt_server_port = 1883;

////////////////////////////////////


// Elements

// Wifi
WiFiClient espClient;

// DHT
DHT dht(15, DHT22);

// MQTT
PubSubClient mqttClient(espClient);


void setup() {
  Serial.begin(115200);
  dht.begin();

  setup_wifi();

  mqttClient.setServer(mqtt_server_host, mqtt_server_port);
  String client_id = "esp32-client-" + String(WiFi.macAddress());
  Serial.printf("\nThe client %s attempting connection to the public MQTT broker\n", client_id.c_str());

  while (!mqttClient.connected()) {
    if (mqttClient.connect(client_id.c_str())) {
      Serial.print(mqttClient.state());
      Serial.println("\nConnection established with Broker");
    } else {
        delay(500);
        Serial.print(".");
    }
  }
}

// the loop function runs over and over again forever
void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  char tempString[8];
  dtostrf(t, 1, 2, tempString);
  mqttClient.publish("esp32/temperature", tempString);

  char humString[8];
  dtostrf(h, 1, 2, humString);
  mqttClient.publish("esp32/humidity", humString);

  delay(2000);
}


// Auxiliar Functions

// Wifi
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi IP: " + WiFi.localIP());
}

String Get_WiFiStatus(int Status){
    switch(Status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
}

void scan_networks() {
  int n = WiFi.scanNetworks();
  for (int i = 0 ; i <n ; i++) {
    Serial.printf("\n %-32.32s", WiFi.SSID(i).c_str());
  }
}
