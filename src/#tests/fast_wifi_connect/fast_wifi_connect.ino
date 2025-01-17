#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "HAIDEE123";
const char* password = "harmony2021";
// Set web server port number to 80
WiFiServer server(80);

// Set your Static IP address
IPAddress local_IP(192, 168, 15, 5);
// Set your Gateway IP address
IPAddress gateway(192, 168, 15, 1);

IPAddress subnet(255, 255, 0, 0);

void setup() {
  long startMs = millis();
  Serial.begin(115200);
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (WiFi.SSID() != ssid) {
    Serial.println(F("Initialising Wifi..."));
    WiFi.begin(ssid, password);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
  }
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("From power to wifi:");
  Serial.println(millis() - startMs);

}
void loop(){
  
}
