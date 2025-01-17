void controlAll(uint8_t value){
    analogWrite(OUT0, value);
    analogWrite(OUT1, value);
    analogWrite(OUT2, value);
    analogWrite(OUT3, value);
    digitalWrite(AUXILIAR_LED, value>0);
}
void blinkAll(uint8_t amount, int period, uint8_t value){
  for(uint8_t i =0; i<amount; i++){
    controlAll(value);
    delay(period/2);
    controlAll(0);
    delay(period/2);
  }
}
