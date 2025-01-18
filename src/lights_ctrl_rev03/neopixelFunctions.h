#ifndef NEOPIXEL_FUNCTIONS_H__
#define NEOPIXEL_FUNCTIONS_H__
#include <arduino.h>
#include "board_config.h"
#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>


extern uint8_t RGBW[];
extern bool out_d[];
extern String loopQueue;
extern String shiftQueue;
extern ESP8266WebServer server;
extern Adafruit_NeoPixel *pixels;

extern String ledQueue;// = "10.0,250,0,0,0,119;10.0,0,250,0,119,0,10.250,0,0,0,0,119,10.0,0,0,250,119,0,10.1,0,0,0,0,119,END";


void toggleNpWhite(uint8_t value);
void npAllPixels();
void neopixel_setPixels();
String shiftNp(String original, int shift);

#endif