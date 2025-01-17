#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "";
const char* pwd = "";
byte ip[] = {192, 168, 15, 5};
byte gw[] = {192, 168, 15, 1};
byte mask[] = {255, 255, 255, 0};
byte dns[] = {0};
byte bssid[] = {0x98,0x7E,0xCA,0x5D,0xFF,0xBF};
byte chnl = 6;

HTTPClient http;
WiFiClient client;

IPAddress getIpAddrFromArr(byte ipAddr[]) {
  return IPAddress(ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
}

void connectToWifi(String ssid, String pwd, byte ip[], byte gw[], byte mask[], byte dns[], byte bssid[], byte chnl ) {
  //if (WiFi.status() == WL_CONNECTED) return;
  Serial.println(F("Connecting wifi..."));
  WiFi.enableSTA(true);
  WiFi.mode(WIFI_STA);
  if (ip[0] != 0) WiFi.config(getIpAddrFromArr(ip), getIpAddrFromArr(gw), getIpAddrFromArr(mask), getIpAddrFromArr(dns));
//  if ((ssid.length() != 0) && (pwd.length() == 0)) {
//    WiFi.begin(ssid.c_str());
//  } else if ((ssid.length() != 0) && (pwd.length() != 0)) {
//    if (bssid[0] == 0) {
//      WiFi.begin(ssid.c_str(), pwd.c_str());
//    } else {
      WiFi.begin(ssid.c_str(), pwd.c_str(), chnl, bssid);
//    }
    WiFi.setAutoConnect(true);
//  }
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (++attempts < 300) {
      Serial.print(".");
      delay(50);
    } else break;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print(F("ms. IP: "));
      Serial.print(WiFi.localIP());
    /*for (byte thisByte = 0; thisByte < 4; thisByte++) {
      if (thisByte != 3) Serial.print(".");
    }*/
    Serial.println();
  }
}

void setup() {
  long st = millis();
  Serial.begin(115200);
  Serial.println(F("Connecting wifi..."));
  connectToWifi(ssid, pwd, ip, gw, mask, dns, bssid, chnl);

  http.begin(client, "http://192.168.15.23/out3");
  http.GET();
  http.end();
  Serial.println("-------");
    Serial.print(F("\nConnected to AP in "));
  Serial.print(millis() - st, DEC);
  ESP.deepSleep(0); 
}

void loop() {
}
