#include "alexaConfig.h"

void alexaOut1(uint8_t brightness) {
  analogWrite(OUT1, brightness);
}
void alexaOut2(uint8_t brightness) {
  analogWrite(OUT2, brightness);
}
void alexaOut3(uint8_t brightness) {
  analogWrite(OUT3, brightness);
}
void alexaOut4(uint8_t brightness) {
  toggleNpWhite(brightness);
}
void alexaOut5(uint8_t brightness) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}


void addDevices(){
  espalexa.addDevice("L um", alexaOut1);
  espalexa.addDevice("L dois", alexaOut2);
  espalexa.addDevice("L tres", alexaOut3);
  espalexa.addDevice("L quatro", alexaOut4);
  espalexa.addDevice("L cinco", alexaOut5);
  espalexa.begin();
}