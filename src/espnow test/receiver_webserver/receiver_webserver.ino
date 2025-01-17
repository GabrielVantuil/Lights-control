#include <ESP8266WiFi.h>
#include <espnow.h>
//#include <esp_wifi.h>



#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

typedef struct struct_message {
    char cmd[4];
    int start;
    int end;
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t W;
} struct_message;

// Create a struct_message called myData
struct_message myData;

#define STASSID ""
#define STAPSK  ""
const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.cmd);
  Serial.println(strcmp(myData.cmd, "OUT1"));
  if(!strcmp(myData.cmd, "OUT1"))  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);


  
  Serial.println("initializing");
  Serial.println(WiFi.localIP());
  delay(100);
  server.on("/out1", []() {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    server.send(200, "text/plain", "this works as well");
  });
  server.begin();
  Serial.println(WiFi.channel());
}

void loop() {
  server.handleClient();
  MDNS.update();
}
