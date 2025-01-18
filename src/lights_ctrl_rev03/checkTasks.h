#ifndef CHECK_TASKS_H__
#define CHECK_TASKS_H__
#include <arduino.h>
#include "board_config.h"
#include "neopixelFunctions.h"

extern String loopQueue;
extern String shiftQueue;
extern unsigned long count_t;
extern unsigned long last_t;
extern bool isShift;
extern int timesToShift;
extern double freq;
extern uint8_t PWM_out[];
extern bool blinking;


void checkNpQueue();
void checkLeds();

#endif