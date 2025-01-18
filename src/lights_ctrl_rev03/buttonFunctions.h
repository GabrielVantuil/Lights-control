#ifndef BUTTON_FUNCTIONS_H__
#define BUTTON_FUNCTIONS_H__
#include <arduino.h>
#include "board_config.h"
#include "neopixelFunctions.h"
#include "handlers.h"

#define INC_DEC 60
#define MIN_PRESSING_DIM 800
const uint8_t PWM_presets[] = { 5, 10, 40, 90, 180, 255 };
extern const uint8_t presets_last_index;

extern unsigned long bt_pressed_ts[];
extern bool bt_start_edge[];
extern bool bt_is_pressed[];
extern bool is_dimming[];
extern uint8_t PWM_out[];
extern uint8_t RGBW[];
extern bool out_d[];
extern bool bt_prev[];
extern uint8_t ch_presets[];
extern uint8_t RGB_presets[];
extern String loopQueue;
extern String shiftQueue;
extern int timesToShift;
extern String ledQueue;
extern unsigned long bt_timeout;

void bt_functions();
void bt_released(uint8_t bt);
void bt_released_RGB(int bt);
void check_buttons();

#endif
