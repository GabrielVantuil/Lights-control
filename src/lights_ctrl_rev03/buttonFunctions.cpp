#include "buttonFunctions.h"

void bt_functions(){
  for(uint8_t bt = 0; bt<4; bt++){
    if(bt_start_edge[bt]){
      bt_pressed_ts[bt] = millis();
      bt_is_pressed[bt] = true;
      bt_start_edge[bt] = false;
    }
    if(digitalRead(ALT_BUTTON) || (digitalRead(BUTTON[3]) && !digitalRead(ALT_BUTTON))){      //editing RGB
      if(bt_is_pressed[bt] && (millis() - bt_pressed_ts[bt]) > MIN_PRESSING_DIM){
        is_dimming[bt] = true;
        bt_pressed_ts[bt] += MIN_PRESSING_DIM;
        RGB_presets[bt] =  (RGB_presets[bt] + presets_last_index) % (presets_last_index+1); //cycles through all presets, decreasing, when reaches zero returns
        RGBW[bt] = PWM_presets[RGB_presets[bt]];
        if(out_d[3])  npAllPixels();
      }
    }
    else{
      if(bt_is_pressed[bt] && (millis() - bt_pressed_ts[bt]) > MIN_PRESSING_DIM){
        is_dimming[bt] = true;
        bt_pressed_ts[bt] += MIN_PRESSING_DIM;
        ch_presets[bt] =  (ch_presets[bt] + presets_last_index) % (presets_last_index+1); //cycles through all presets, decreasing, when reaches zero returns
        PWM_out[bt] = PWM_presets[ch_presets[bt]];
      }
    }
  }
}

void bt_released(uint8_t bt){
  if(!is_dimming[bt] && (millis() - bt_pressed_ts[bt]) < MIN_PRESSING_DIM){
    out_d[bt] = !out_d[bt];   
    bt_prev[bt] = false;
  }
  is_dimming[bt] = false;
  bt_is_pressed[bt] = false;
}
void bt_released_RGB(int bt){
  if(bt==-1){
    clearLeds();
    return;
  }
  if(!is_dimming[bt] && (millis() - bt_pressed_ts[bt]) < MIN_PRESSING_DIM){
    RGBW[bt] = 255*(RGBW[bt]==0);
    bt_prev[bt] = false;
  }
  npAllPixels();

  is_dimming[bt] = false;
  bt_is_pressed[bt] = false;
}

void check_buttons(){
  
  if(digitalRead(BUTTON[0]) && bt_prev[0]) {bt_start_edge[0] = true;}
  if(digitalRead(BUTTON[1]) && bt_prev[1]) {bt_start_edge[1] = true;}
  if(digitalRead(BUTTON[2]) && bt_prev[2]) {bt_start_edge[2] = true;}
  if(digitalRead(BUTTON[3]) && bt_prev[3]) {bt_start_edge[3] = true;}
  
  if(!digitalRead(ALT_BUTTON)){   //not editing RGB
    if(!digitalRead(BUTTON[0]) && bt_is_pressed[0]) {bt_released(0);}
    if(!digitalRead(BUTTON[1]) && bt_is_pressed[1]) {bt_released(1);}
    if(!digitalRead(BUTTON[2]) && bt_is_pressed[2]) {bt_released(2);}
    if(!digitalRead(BUTTON[3]) && bt_is_pressed[3]) {bt_released_RGB(3);}
  }
  else{
    if(!digitalRead(BUTTON[0]) && bt_is_pressed[0]) {bt_released_RGB(0);}
    if(!digitalRead(BUTTON[1]) && bt_is_pressed[1]) {bt_released_RGB(1);}
    if(!digitalRead(BUTTON[2]) && bt_is_pressed[2]) {bt_released_RGB(2);}
    if(!digitalRead(BUTTON[3]) && bt_is_pressed[3]) {bt_released_RGB(-1);}
  
  }
  bt_functions();
  
  if((millis()-bt_timeout) > 100){
    bt_timeout = millis();
    bt_prev[0] = !digitalRead(BUTTON[0]);
    bt_prev[1] = !digitalRead(BUTTON[1]);
    bt_prev[2] = !digitalRead(BUTTON[2]);
    bt_prev[3] = !digitalRead(BUTTON[3]);
  }
}
