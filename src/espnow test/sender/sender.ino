#include <ESP8266WiFi.h>
#include <espnow.h>
#include <EEPROM.h>
uint8_t broadcastAddress[] = {0xA0, 0x20, 0xA6, 0x06, 0x7F, 0x45};  //test esp
//uint8_t broadcastAddress[] = {0x2C, 0x3A, 0xE8, 0x0F, 0x0D, 0xB1};  //monitor esp

typedef struct struct_message {
    char cmd[4];
    int start;
    int end;
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t W;
} struct_message;

struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 1000;  // send readings timer

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
//  EEPROM.begin(512);
//  bool val = !EEPROM.read(0);
//  EEPROM.write(0, val);
//  EEPROM.commit();
  pinMode(LED_BUILTIN,OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  
    strcpy(myData.cmd, "OUT1");
    myData.start = 0;
    myData.end = 0;
    myData.R = 0;
    myData.G = 0;
    myData.B = 0;
    myData.W = 0;
//    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//    delay(1000);
//    strcpy(myData.cmd, "OUT2");
//    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//    ESP.deepSleep(0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    strcpy(myData.cmd, "OUT1");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    lastTime = millis();
  }
}
