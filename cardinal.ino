
#include <ESP8266WiFi.h>

const char* ssid     = ""; // not public of course :) if someone know how to
const char* password = ""; // keep secret this, open an issue to help me.

unsigned long previousMillis = 0;
const long interval = 5000;

void connectToWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  connectToWifi();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if (WiFi.status() != WL_CONNECTED) {
      connectToWifi();
    }
    Serial.println("loop end");
    
    previousMillis = currentMillis;
  }
}
