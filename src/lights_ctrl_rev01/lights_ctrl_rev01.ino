#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* host = "lights";
const char* ssid = STASSID;
const char* password = STAPSK;


#define OUT1 13
#define OUT2 12
#define OUT3 14
#define OUT4 16
#define IR_LED 4
#define NEOPIXEL 5

int numPixels   = 120;


//------------------ neopixel--------------------
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

const int neopixel_pin =  NEOPIXEL;
int pixelFormat = NEO_GRBW + NEO_KHZ800;

Adafruit_NeoPixel *pixels;

//------------------ HTTP--------------------

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

const int led = 2;
String filaLeds;

void setup(void) {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels = new Adafruit_NeoPixel(numPixels, neopixel_pin, pixelFormat);
  pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  pixels->setPixelColor(10, pixels->Color(255,255,255,255));
  delay(1000);
  pixels->clear(); // Set all pixel colors to 'off'

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(2, OUTPUT);
  analogWrite(2, 0);
  
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
//
//  if (MDNS.begin("esp8266")) {
//    Serial.println("MDNS responder started");
//  }
  

  //MDNS.begin(host);

  httpUpdater.setup(&server);

  //MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  //server.on("/", handleRoot);
  server.on("/OUT1", d_Out1);
  server.on("/OUT2", d_Out2);
  server.on("/OUT3", d_Out3);
  server.on("/OUT4", d_Out4);
  server.on("/clear", []() {
    pixels->clear();
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
  analogWrite(2, 254);  
}
int delayNext, startLed, endLed, filaR, filaG, filaB, filaW, nextLed;
void loop(void) {
  server.handleClient();
  //MDNS.update();

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
        filaW = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        startLed = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        endLed = filaLeds.substring(0, filaLeds.indexOf(',') + 1).toInt();
        filaLeds.remove(0, filaLeds.indexOf(',') + 1);
        nextLed = startLed;
      }
    }
    else{
      nextLed += abs(endLed - startLed) / (endLed - startLed);
    }
    pixels->setPixelColor(nextLed, pixels->Color(filaR, filaG, filaB, filaW));
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

void d_Out1(){digitalWrite(OUT1, !digitalRead(OUT1)); server.send(200, "text/plain", "Command accepted");}
void d_Out2(){digitalWrite(OUT2, !digitalRead(OUT2)); server.send(200, "text/plain", "Command accepted");}
void d_Out3(){digitalWrite(OUT3, !digitalRead(OUT3)); server.send(200, "text/plain", "Command accepted");}
void d_Out4(){digitalWrite(OUT4, !digitalRead(OUT4)); server.send(200, "text/plain", "Command accepted");}