#ifndef HANDLERS_H__
#define HANDLERS_H__
#include <ESP8266WebServer.h>
#include "neopixelFunctions.h"

extern const uint8_t presets_last_index;

extern uint8_t RGBW[];
extern bool out_d[];
extern String loopQueue;
extern String shiftQueue;
extern bool bt_is_pressed[];
extern bool is_dimming[];
extern uint8_t PWM_out[];
extern uint8_t RGBW[];
extern bool bt_prev[];
extern int timesToShift;
extern String ledQueue;
extern uint8_t ch_presets[];
extern ESP8266WebServer server;
extern bool isShift;
extern String MAC_ADDR;

void digitalOut();
void analogOutX(uint8_t out);
void RGB_handle(uint8_t rgb);

void d_Out1();
void d_Out2();
void d_Out3();
void d_Out4();

void np_R();
void np_G();
void np_B();


void clearLeds();
void genericOut_handler();
String strMac();

void handleRoot();
void info();

void handleNotFound();

void ping();

void help();

#endif