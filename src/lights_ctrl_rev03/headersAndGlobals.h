#ifndef HEADERS_AND_GLOBALS_H__
#define HEADERS_AND_GLOBALS_H__ 

const uint8_t presets_last_index = 5;

unsigned long count_t = 0;
unsigned long last_t = 0;
double freq = 20;
float duty = 0.1;
bool blinking = false;

unsigned long bt_timeout = 0;
uint8_t ctrl_ch = 4;
bool out_d[] = { false, false, false, false };
uint8_t RGBW[] = { 0, 0, 0, 0 };
bool before_pause[4];

bool bt_prev[] = { false, false, false, false };
unsigned long bt_pressed_ts[] = { 0, 0, 0, 0 };  //button down timestamp
bool bt_start_edge[] = { false, false, false, false };
bool bt_is_pressed[] = { false, false, false, false };
bool is_dimming[] = { false, false, false, false };
uint8_t PWM_out[] = { 255, 255, 255, 255 };
bool isShift = false;
int timesToShift = 0;
uint8_t ch_presets[] = { presets_last_index, presets_last_index, presets_last_index, presets_last_index, 0 };
uint8_t RGB_presets[] = { presets_last_index, presets_last_index, presets_last_index, presets_last_index, 0 };
String MAC_ADDR;
String ledQueue;

String loopQueue;
String shiftQueue;

ESP8266WebServer server = ESP8266WebServer(80);

Adafruit_NeoPixel *pixels;
Espalexa espalexa;

void neopixel_setpixelRange();
void neopixel_pixelAnimacao();
void initWifi(const char* ssid, const char* password);
//handlers
void digitalOut();
void d_Out1();
void d_Out2();
void d_Out3();
void d_Out4();
void clearLeds();
void genericOut_handler();
void handleRoot();
void handleNotFound();
void ping();
void help();
void onEspNowDataRecv(uint8_t* mac, uint8_t* incomingData, uint8_t len);
#endif
