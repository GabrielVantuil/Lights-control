#include <ESP8266WiFi.h>
#include <espnow.h>
//#include <esp_wifi.h>
#include <ESP8266HTTPClient.h>


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

HTTPClient http;
WiFiClient client;

// Set your Static IP address
IPAddress local_IP(192, 168, 15, 5);
// Set your Gateway IP address
IPAddress gateway(192, 168, 15, 1);

IPAddress subnet(255, 255, 0, 0);

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
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);


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
  
  http.begin(client, "http://192.168.15.100/out3");
  http.GET();
  http.end();
}

void loop() {
  delay(100);
}
