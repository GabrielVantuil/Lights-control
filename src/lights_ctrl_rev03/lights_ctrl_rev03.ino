#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "headersAndGlobals.h"
#include <espnow.h>
#include "board_config.h"
//#include <ESP8266HTTPUpdateServer.h>
/*
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
*/
#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* host = "lights";
const char* ssid = STASSID;
const char* password = STAPSK;
#define HOST_NAME "lights ctrl"
String MAC_ADDR;

//#define ENABLE_LOG

//------------------ neopixel--------------------
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

Adafruit_NeoPixel *pixels;

//------------------ HTTP--------------------

//AsyncWebServer server(80);
ESP8266WebServer server(80);
//ESP8266HTTPUpdateServer httpUpdater;

String ledQueue;// = "10.0,250,0,0,0,119;10.0,0,250,0,119,0,10.250,0,0,0,0,119,10.0,0,0,250,119,0,10.1,0,0,0,0,119,END";
String loopQueue;
String shiftQueue;
void setup(void) {
  pinMode(OUT0, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(BUTTON0, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  digitalWrite(OUT0, LOW);
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, LOW);
  digitalWrite(OUT3, LOW);
  pinMode(AUXILIAR_LED, OUTPUT);
  analogWrite(AUXILIAR_LED, 100);

  pixels = new Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);
  pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels->clear(); // Set all pixel colors to 'off'

  #if defined(ENABLE_LOG)
  Serial.begin(115200);
  #endif
//  delay(1000);
//  Serial.println(shiftNp("S1;0.0,0,0,0f1t119;0.0,0,0,0f1t119;END", 20));
//  Serial.println(shiftNp("S1;0.0,0,0,0f1t119;0.0,0,0,0f113t9;END", -20));
//  Serial.println(shiftNp("S1;0.0,0,0,0f101t219;0.0,0,0,0f213t109;END", -20));
  
  initWifi(ssid, password);
  pixels->clear();
  pixels->show();
  MDNS.begin(host);
  MDNS.addService("http", "tcp", 80);

  setHandlers();  
  server.begin();

  analogWrite(2, 254);  
}


void loop(void) {
  server.handleClient();
  MDNS.update();
  checkNpQueue();
  checkLeds();
  check_buttons();
}
