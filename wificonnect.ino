#include <WiFi.h>

const char* ssid = "obdo";
const char* password = "mark31:5";

WiFiClient client;
const int port = 80;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    if (client.connect("10.42.0.1", port)) { // Replace with your laptop's IP address
      Serial.println("Connected to server");
      client.println("Hello from ESP32");
    }
  }
  delay(1000);
}