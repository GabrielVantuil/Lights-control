#include <IRremote.h>
#define RECV_PIN 7

#define OUT0 9
#define OUT1 6
#define OUT2 5
#define OUT3 3

#define BUTTON0 A0
#define BUTTON1 A4
#define BUTTON2 A3
#define BUTTON3 A5
#define AUXILIAR_LED 13
const int BUTTON[] = {BUTTON0,BUTTON1,BUTTON2,BUTTON3};

#define INC_DEC 60
#define MIN_PRESSING_DIM 800

const uint8_t PWM_presets[]= {5,10,40,90,180,255};
#define presets_last_index 5


#define CMD_1       0xE0E020DF  //1
#define CMD_2       0xE0E0A05F  //2
#define CMD_3       0xE0E0609F  //3
#define CMD_4       0xE0E010EF  //4
#define CMD_5       0xE0E0906F  //5 
#define CMD_6       0xE0E050AF  //6
#define CMD_7       0xE0E030CF  //7
#define CMD_8       0xE0E0B04F  //8
#define CMD_9       0xE0E0708F  //9
#define CMD__       0xE0E0C43B  //-
#define CMD_0       0xE0E08877  //0 
#define CMD_ENTER   0xE0E016E9  //enter or OK
#define CMD_UP      0xE0E006F9    //up
#define CMD_RIGHT   0xE0E046B9    //right
#define CMD_LEFT    0xE0E0A659    //left
#define CMD_DOWN    0xE0E08679    //down
#define CMD_CUBE    0xE0E09E61    //cube symbol
#define CMD_REC     0xE0E0926D    //rec (red circle)

#define CMD_YELLOW  0xE0E0A857  //yellow
#define CMD_BLUE    0xE0E06897  //blue
#define CMD_RED     0xE0E036C9  //red
#define CMD_GREEN   0xE0E028D7  //green

#define CMD_PLAY    0xE0E0E21D  //play
#define CMD_PAUSE   0xE0E052AD  //pause
#define CMD_STOP    0xE0E0629D  //stop



IRrecv irrecv(RECV_PIN);
decode_results results;
void setup() {
  
    irrecv.enableIRIn(); // Start the receiver
    Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(OUT0, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(AUXILIAR_LED, OUTPUT);
  
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
}

long IR_cmd=0;
long IR_cmd_first=0;
double freq = 20;
float duty = 0.1;
uint8_t ch_presets[] = {presets_last_index, presets_last_index, presets_last_index, presets_last_index, 0};
bool blinking = false;
unsigned long count_t =0;
unsigned long last_t =0;
bool waiting_cmd = false;
unsigned long cmd_timeout=0;
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
void check_buttons(){
  
  if(!digitalRead(BUTTON[0]) && bt_prev[0]) {bt_start_edge[0] = true;}
  if(!digitalRead(BUTTON[1]) && bt_prev[1]) {bt_start_edge[1] = true;}
  if(!digitalRead(BUTTON[2]) && bt_prev[2]) {bt_start_edge[2] = true;}
  if(!digitalRead(BUTTON[3]) && bt_prev[3]) {bt_start_edge[3] = true;}

  if(digitalRead(BUTTON[0]) && bt_is_pressed[0]) {bt_released(0);}
  if(digitalRead(BUTTON[1]) && bt_is_pressed[1]) {bt_released(1);}
  if(digitalRead(BUTTON[2]) && bt_is_pressed[2]) {bt_released(2);}
  if(digitalRead(BUTTON[3]) && bt_is_pressed[3]) {bt_released(3);}
  
  bt_functions();
  
  if((millis()-bt_timeout) > 100){
    bt_timeout = millis();
    bt_prev[0] = digitalRead(BUTTON[0]);
    bt_prev[1] = digitalRead(BUTTON[1]);
    bt_prev[2] = digitalRead(BUTTON[2]);
    bt_prev[3] = digitalRead(BUTTON[3]);
  }
}
void loop() {
  check_buttons();
  if(irrecv.decode(&results)) {
//    IR_cmd = translateIR(results.value);
    IR_cmd = results.value;
    if(IR_cmd_first != IR_cmd){
      IR_cmd_first = IR_cmd;
      IR_cmd = 0;
    }
    else {
      IR_cmd_first = 0;
      Serial.println(IR_cmd);
      Serial.println(results.value, HEX);
      if(waiting_cmd)
        blinkAll(1,100,20);
    }
    cmd_timeout = millis();
    irrecv.resume(); // Receive the next value
  }
  if(IR_cmd == CMD_CUBE){
    waiting_cmd = true;
    cmd_timeout = millis();
    blinkAll(1,100,10);
    Serial.println("IN");
  }
  if(waiting_cmd && (millis() > (cmd_timeout + 5000))){
    waiting_cmd = false;
    blinkAll(1,100,10);
    Serial.println("out");
    ctrl_ch = 4;
  }
  
  if(waiting_cmd){
    if(IR_cmd == CMD_REC)
      blinking = !blinking;
    if(IR_cmd == CMD_YELLOW){
      out_d[0] = !out_d[0];
      ctrl_ch = 0;
    }
    else if(IR_cmd == CMD_BLUE){
      out_d[1] = !out_d[1];
      ctrl_ch = 1;
    }
    else if(IR_cmd == CMD_RED){
      out_d[2] = !out_d[2];
      ctrl_ch = 2;
    }
    else if(IR_cmd == CMD_GREEN){
      out_d[3] = !out_d[3];
      ctrl_ch = 3;
    }

    else if(IR_cmd == CMD_UP){
      if(ctrl_ch != 4)
        ch_presets[ctrl_ch]+=ch_presets[ctrl_ch]!=presets_last_index;
      else{
        ch_presets[0]+=ch_presets[0]!=presets_last_index;
        ch_presets[1]+=ch_presets[1]!=presets_last_index;
        ch_presets[2]+=ch_presets[2]!=presets_last_index;
        ch_presets[3]+=ch_presets[3]!=presets_last_index;
      }
    }
    else if(IR_cmd == CMD_DOWN){
      if(ctrl_ch != 4)
        ch_presets[ctrl_ch]-=ch_presets[ctrl_ch]!=0;
      else{
        ch_presets[0]-=ch_presets[0]!=0;
        ch_presets[1]-=ch_presets[1]!=0;
        ch_presets[2]-=ch_presets[2]!=0;
        ch_presets[3]-=ch_presets[3]!=0;
      }
    }

    else if(IR_cmd == CMD_PLAY){
      out_d[0] = true;
      out_d[1] = true;
      out_d[2] = true;
      out_d[3] = true;
      ch_presets[0] = presets_last_index;
      ch_presets[1] = presets_last_index;
      ch_presets[2] = presets_last_index;
      ch_presets[3] = presets_last_index;
    }
    else if(IR_cmd == CMD_PAUSE){
      if(out_d[0]||out_d[1]||out_d[2]||out_d[3]){  //if any light is on, save all status and turn it off
        before_pause[0] = out_d[0];
        before_pause[1] = out_d[1];
        before_pause[2] = out_d[2];
        before_pause[3] = out_d[3];
        out_d[0] = false;
        out_d[1] = false;
        out_d[2] = false;
        out_d[3] = false;
      }
      else{ //retrieve all status
        out_d[0] = before_pause[0];
        out_d[1] = before_pause[1];
        out_d[2] = before_pause[2];
        out_d[3] = before_pause[3];
      }
    }
    else if(IR_cmd == CMD_STOP){
      out_d[0] = false;
      out_d[1] = false;
      out_d[2] = false;
      out_d[3] = false;
      ch_presets[0] = 0;
      ch_presets[1] = 0;
      ch_presets[2] = 0;
      ch_presets[3] = 0;
    }
  }
  if(count_t >= (int)(1000.0/freq)){
    last_t=millis();
    analogWrite(OUT0, PWM_presets[ch_presets[0]] *out_d[0]);
    analogWrite(OUT1, PWM_presets[ch_presets[1]] *out_d[1]);
    analogWrite(OUT2, PWM_presets[ch_presets[2]] *out_d[2]);
    analogWrite(OUT3, PWM_presets[ch_presets[3]] *out_d[3]);
    digitalWrite(AUXILIAR_LED, HIGH);
//    delay((int)(500.0/freq*duty));
//    delay((int)(500.0/freq)*(1.0-duty));
  }
  //if(count_t >= ((int)(1000.0*duty/freq)) && blinking){ //OFF In case of blinking mode
  else if(blinking){ //OFF In case of blinking mode
    analogWrite(OUT0, 0);
    analogWrite(OUT1, 0);
    analogWrite(OUT2, 0);
    analogWrite(OUT3, 0);
    digitalWrite(AUXILIAR_LED, LOW);
  }

  count_t = millis()-last_t;
  IR_cmd = 0;
  
}
