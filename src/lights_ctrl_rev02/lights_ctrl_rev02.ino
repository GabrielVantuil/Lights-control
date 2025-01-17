#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <ESP8266HTTPUpdateServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#ifndef STASSID
#define STASSID "HAIDEE123"
#define STAPSK  "harmony2021"
#endif

const char* host = "lights";
const char* ssid = STASSID;
const char* password = STAPSK;


#define OUT1 13
#define OUT2 12
#define OUT3 14
#define OUT4 16 //neopixel's mosfet
#define IR_LED 4
#define NEOPIXEL 5

int numPixels   = 120;


#define VALUE_BTN1 496
#define VALUE_BTN2 247
#define VALUE_BTN3 128
#define VALUE_BTN4 67
#define VALUE_BTN5 41

#define A_OFFSET 12
#define A_RELEASE 20

//------------------ neopixel--------------------
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

const int neopixel_pin =  NEOPIXEL;
int pixelFormat = NEO_GRBW + NEO_KHZ800;

Adafruit_NeoPixel *pixels;

//------------------ HTTP--------------------

AsyncWebServer server(80);
//ESP8266WebServer server(80);
//ESP8266HTTPUpdateServer httpUpdater;

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
  digitalWrite(OUT4, HIGH);
  
  initWifi(ssid, password);

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

  //httpUpdater.setup(&server);

  //MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  //server.on("/", handleRoot);
  /*server.on("/OUT1", HTTP_GET, d_Out1);
  server.on("/OUT2", HTTP_GET, d_Out2);
  server.on("/OUT3", HTTP_GET, d_Out3);
  server.on("/OUT4", HTTP_GET, d_Out4);
  server.on("/clear", HTTP_GET, []() {
    pixels->clear();
    server.send(200, "text/plain", "this works as well");
  });*/
  server.on("/digitalOut", HTTP_GET, digitalOut);
  //server.on("/setpixel", HTTP_GET, neopixel_setpixel);
  server.on("/setpixelRange", HTTP_GET, neopixel_setpixelRange);/*
  server.on("/pixelAnimacao", HTTP_GET, neopixel_pixelAnimacao);
  server.on("/ping", HTTP_GET, ping);
  server.on("/help", HTTP_GET, help);*/
//  server.onNotFound(handleNotFound);
  //---------------OTA------------------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP8266.");
  });
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  //------------------------------------------
  server.begin();
  Serial.println("HTTP server started");
  analogWrite(2, 254);  
}

int delayNext, startLed, endLed, filaR, filaG, filaB, filaW, nextLed;
void loop(void) {
  //server.handleClient();
  //MDNS.update();
  if(analogRead(A0)>=VALUE_BTN5-5)
    convAnalogBTN();
  delay(10);
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
int getAnalogRead(){
  #define A_SAMPLES 500
  int value = analogRead(A0);
  double sum = value;
  int avg = sum;
  long count = 1;
  //while(((value = analogRead(A0)) > A_RELEASE) && (value>(avg*0.9))){
  while(((value = analogRead(A0)) > A_RELEASE) && (value>(avg*0.9)) && count<A_SAMPLES){
    sum += value;
    count++;
    avg = sum/count;
  }
  return avg;
}
int checkBt(){
  long timeStart = millis();
  int value = getAnalogRead();
  long elapsedTime = millis()- timeStart;
  Serial.print("pushed for: ");
  Serial.print(elapsedTime);
  Serial.print("\t");
  Serial.print(value);
  Serial.print("\t");
  
  if( (value > (VALUE_BTN1-A_OFFSET)) && (value < (VALUE_BTN1+A_OFFSET)) ){
    Serial.print("BTN 1 \t");
    value-=VALUE_BTN1;
  }
  if( (value > (VALUE_BTN2-A_OFFSET)) && (value < (VALUE_BTN2+A_OFFSET)) ){
    Serial.print("BTN 2 \t");
    value-=VALUE_BTN2;
  }
  if( (value > (VALUE_BTN3-A_OFFSET)) && (value < (VALUE_BTN3+A_OFFSET)) ){
    Serial.print("BTN 3 \t");
    value-=VALUE_BTN3;
  }
  if( (value > (VALUE_BTN4-A_OFFSET)) && (value < (VALUE_BTN4+A_OFFSET)) ){
    Serial.print("BTN 4 \t");
    value-=VALUE_BTN4;
  }
  if( (value > (VALUE_BTN5-A_OFFSET)) && (value < (VALUE_BTN5+A_OFFSET)) ){
    Serial.print("BTN 5 \t");
  }
  Serial.print("\n");
}

void digitalOut(AsyncWebServerRequest *request) {
  String arguments = "Comando(s) aceito(s): \n";
  for (uint8_t i = 0; i < request->args(); i++) {
    if (request->argName(i) == "change") {
      digitalWrite(request->arg(i).toInt(), !digitalRead(request->arg(i).toInt()));
      arguments += String(i) + ") Status da porta " + request->arg(i).toInt() + " alterado para: " + ((digitalRead(request->arg(i).toInt()) == 0) ? "LOW" : "HIGH") + "\n";
    }
    else {
      arguments += String(i) + ") Enviando para porta " + request->argName(i) + ": " + ((request->arg(i).toInt() == 0) ? "LOW" : "HIGH") + "\n";
      digitalWrite(request->argName(i).toInt(), request->arg(i).toInt() == 0 ? LOW : HIGH);
    }
  }
  Serial.println(arguments);

  request->send(404, "text/plain", arguments);
}
/*
void d_Out1(){digitalWrite(OUT1, !digitalRead(OUT1)); server.send(200, "text/plain", "Command accepted");}
void d_Out2(){digitalWrite(OUT2, !digitalRead(OUT2)); server.send(200, "text/plain", "Command accepted");}
void d_Out3(){digitalWrite(OUT3, !digitalRead(OUT3)); server.send(200, "text/plain", "Command accepted");}
void d_Out4(){digitalWrite(OUT4, !digitalRead(OUT4)); server.send(200, "text/plain", "Command accepted");}
*/
