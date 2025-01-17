/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//------------------ neopixel--------------------
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

const int neopixel_pin =  13;
int numPixels   = 50;
int pixelFormat = NEO_RBG + NEO_KHZ800;

Adafruit_NeoPixel *pixels;

//------------------ HTTP--------------------


#define STASSID "HAIDEE123"
#define STAPSK  "harmony2021"
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 2;
String filaLeds;

void setup(void) {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels = new Adafruit_NeoPixel(numPixels, neopixel_pin, pixelFormat);
  pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  pixels->clear(); // Set all pixel colors to 'off'

  pinMode(led, OUTPUT);
  pinMode(5, OUTPUT); //lampada
  digitalWrite(5,HIGH);
  digitalWrite(led, 0);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  //server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.on("/digitalOut", digitalOut);
  server.on("/setpixel", neopixel_setpixel);
  server.on("/setpixelRange", neopixel_setpixelRange);
  server.on("/pixelAnimacao", neopixel_pixelAnimacao);
  server.on("/ping", ping);
  server.on("/help", help);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(led, 1);
}
int delayNext, startLed, endLed, filaR, filaG, filaB, nextLed;
void loop(void) {
  server.handleClient();

  if (filaLeds.length() != 0) {
    if (nextLed == endLed) {
      if (filaLeds == "FIM") {
        filaLeds.remove(0);
      }
      else {
        delayNext = filaLeds.substring(0, filaLeds.indexOf('.') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf('.') + 1);
        filaR = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        filaG = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        filaB = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        startLed = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        endLed = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        nextLed = startLed;
      }
    }
    else
      nextLed += abs(endLed - startLed) / (endLed - startLed);
    pixels->setPixelColor(nextLed, pixels->Color(filaR, filaG, filaB));
    long start = millis();
    while (millis() < start + delayNext);
    pixels->show();
  }
}
void digitalOut() {
  String arguments = "Comando(s) aceito(s): \n";
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "change") {
      digitalWrite(server.arg(i).toInt(), !digitalRead(server.arg(i).toInt()));
      arguments += String(i) + ") Status da porta " + server.arg(i).toInt() + " alterado para: " + ((digitalRead(server.arg(i).toInt()) == 0) ? "LOW" : "HIGH") + "\n";
    }
    else {
      arguments += String(i) + ") Enviando para porta " + server.argName(i) + ": " + ((server.arg(i).toInt() == 0) ? "LOW" : "HIGH") + "\n";
      digitalWrite(server.argName(i).toInt(), server.arg(i).toInt() == 0 ? LOW : HIGH);
    }
  }
  Serial.println(arguments);

  server.send(404, "text/plain", arguments);
}
