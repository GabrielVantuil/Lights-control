
unsigned long count_t =0;
unsigned long last_t =0;
unsigned long bt_timeout=0;
uint8_t ctrl_ch = 4;
bool out_d[] = {false,false,false,false};
bool before_pause[4];

bool bt_prev[] = {false,false,false,false};
unsigned long bt_pressed_ts[]= {0,0,0,0};   //button down timestamp
bool bt_start_edge[] = {false,false,false,false};
bool bt_is_pressed[] = {false,false,false,false};
bool is_dimming[] = {false,false,false,false};


void bt_functions(){
  for(uint8_t bt = 0; bt<4; bt++){
    if(bt_start_edge[bt]){
      bt_pressed_ts[bt] = millis();
      bt_is_pressed[bt] = true;
      bt_start_edge[bt] = false;
    }
    if(bt_is_pressed[bt] && (millis() - bt_pressed_ts[bt]) > MIN_PRESSING_DIM){
      is_dimming[bt] = true;
      bt_pressed_ts[bt] += MIN_PRESSING_DIM;
      ch_presets[bt] =  (ch_presets[bt] + presets_last_index) % (presets_last_index+1); //cycles through all presets, decreasing, when reaches zero returns
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
boolean checkPressed(int bt){
  pressedButton == bt;
}
void check_buttons(){
  
  if(!checkPressed(0) && bt_prev[0]) {bt_start_edge[0] = true;}
  if(!checkPressed(1) && bt_prev[1]) {bt_start_edge[1] = true;}
  if(!checkPressed(2) && bt_prev[2]) {bt_start_edge[2] = true;}
  if(!checkPressed(3) && bt_prev[3]) {bt_start_edge[3] = true;}

  if(checkPressed(0) && bt_is_pressed[0]) {bt_released(0);}
  if(checkPressed(1) && bt_is_pressed[1]) {bt_released(1);}
  if(checkPressed(2) && bt_is_pressed[2]) {bt_released(2);}
  if(checkPressed(3) && bt_is_pressed[3]) {bt_released(3);}
  
  bt_functions();
  
  if((millis()-bt_timeout) > 100){  //
    bt_timeout = millis();
    bt_prev[0] = checkPressed(0);
    bt_prev[1] = checkPressed(1);
    bt_prev[2] = checkPressed(2);
    bt_prev[3] = checkPressed(3);
  }
}
