void initWifi(const char* ssid, const char* password){
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);
  IPAddress local_IP(192, 168, 1, 100);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.hostname(HOST_NAME);
  //WiFi.config(local_IP, gateway, subnet);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  MAC_ADDR = strMac();
  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onEspNowDataRecv);

  
  #if defined(ENABLE_LOG)
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(MAC_ADDR);
  #endif
}
void setHandlers(){
  server.on("/", handleRoot);
  server.on("/out1", d_Out1);
  server.on("/out2", d_Out2);
  server.on("/out3", d_Out3);
  server.on("/out4", d_Out4);
  server.on("/red",   np_R);
  server.on("/green", np_G);
  server.on("/blue",  np_B);
  server.on("/clear", clearLeds);
  server.on("/digitalOut", digitalOut);
  server.on("/setPixels", neopixel_setPixels);
  server.on("/ping", ping);
  server.on("/help", help);
  server.on("/info", info);
  server.onNotFound(handleNotFound);
  
}
/*
void initOTA(){
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
  
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}*/
