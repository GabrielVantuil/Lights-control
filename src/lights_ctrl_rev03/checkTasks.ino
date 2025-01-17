void checkNpQueue(){
  static uint8_t R_val, G_val, B_val, W_val;
  static int startLed, endLed, nextLed;
  static int delayNext;
  static bool hasMaxShift=false;
  static int ledsToShift=0;
  if (ledQueue.length() != 0) {
    // queue exemple: 0.0,250,0,0f0t119;d500,0.0,0,250,0f119t0;d500,0.250,0,0,0f0t119;d500,0.0,0,0,250f119t0;d500,
    if(ledQueue.charAt(0) == 'd'){
      delayNext = ledQueue.substring(1,ledQueue.indexOf(',')).toInt();
      long Start = millis();
      while (millis() < Start + delayNext);
      ledQueue.remove(0, ledQueue.indexOf(',') + 1);
      return;
    }
    if (nextLed == endLed) {
      if(ledQueue.charAt(0) == 'L'){
        ledQueue.remove(0);
        if(isShift) ledQueue.concat(shiftQueue); 
        else        ledQueue.concat(loopQueue);
        ledQueue.concat("L"); 
      }
      if(ledQueue.charAt(0) == 'S'){
        isShift = true;
        ledsToShift = ledQueue.substring(1,ledQueue.indexOf(',')).toInt();
        if(ledQueue.substring(1, ledQueue.indexOf(';')).indexOf(',') != -1){//has two args, so has maximum timesToShift 
          hasMaxShift = true;
          timesToShift = ledQueue.substring(ledQueue.indexOf(',')+1, ledQueue.indexOf(';')).toInt();
          shiftQueue = ledQueue.substring(0, ledQueue.indexOf(',')) + String(timesToShift) + ledQueue.substring(ledQueue.indexOf(';'));
        }
        ledQueue.remove(0, ledQueue.indexOf(';')+1);
        shiftQueue = ledQueue;
        return;
      }
      if (ledQueue.substring(0,3) == "END") {
        if(isShift){
//          pixels->clear();
//          pixels->show();
          if(hasMaxShift)  timesToShift--;
          if(timesToShift<=-1){
            isShift = false;
            hasMaxShift = false;
            timesToShift = 0;
          }
          shiftQueue = shiftNp(shiftQueue, ledsToShift);
          ledQueue = shiftQueue;
          return;
        }
        ledQueue.remove(0,3);
      }else {
        delayNext = ledQueue.substring(0, ledQueue.indexOf('.')).toInt(); ledQueue.remove(0, ledQueue.indexOf('.') + 1);
        R_val = ledQueue.substring(0, ledQueue.indexOf(',')).toInt();     ledQueue.remove(0, ledQueue.indexOf(',') + 1);
        G_val = ledQueue.substring(0, ledQueue.indexOf(',')).toInt();     ledQueue.remove(0, ledQueue.indexOf(',') + 1);
        B_val = ledQueue.substring(0, ledQueue.indexOf(',')).toInt();     ledQueue.remove(0, ledQueue.indexOf(',') + 1);
        W_val = ledQueue.substring(0, ledQueue.indexOf('f')).toInt();     ledQueue.remove(0, ledQueue.indexOf('f') + 1);
        startLed = ledQueue.substring(0, ledQueue.indexOf('t')).toInt();  ledQueue.remove(0, ledQueue.indexOf('t') + 1);
        endLed = ledQueue.substring(0, ledQueue.indexOf(',')).toInt();    ledQueue.remove(0, ledQueue.indexOf(';') + 1);
        nextLed = startLed;
      }
    }
    else{
      //nextLed += abs(endLed - startLed) / (endLed - startLed);
      if((startLed>endLed && startLed>NUMPIXELS) || (startLed<endLed && endLed<NUMPIXELS)) nextLed++;
      else nextLed--;
    }
    //out_d[3] = !((R_val + G_val + B_val + W_val) == 0);
    long Start = millis();
    while (millis() < Start + delayNext);

    if(startLed==endLed){
      pixels->setPixelColor(startLed, pixels->Color(R_val, G_val, B_val, W_val));
      pixels->show();
      return;
    }
    if(delayNext == 0){
      nextLed = endLed;
      if(startLed>endLed){
        pixels->fill(pixels->Color(R_val, G_val, B_val, W_val), endLed%NUMPIXELS, startLed-endLed);
        if(startLed>NUMPIXELS)  pixels->fill(pixels->Color(R_val, G_val, B_val, W_val), 0, startLed-NUMPIXELS);
      }else{
        pixels->fill(pixels->Color(R_val, G_val, B_val, W_val), startLed%NUMPIXELS, endLed-startLed);
        if(endLed>NUMPIXELS)  pixels->fill(pixels->Color(R_val, G_val, B_val, W_val), 0, endLed-NUMPIXELS);
      }
    }
    pixels->setPixelColor(nextLed%NUMPIXELS, pixels->Color(R_val, G_val, B_val, W_val));
    pixels->show();
  }
}
void checkLeds(){
  if(!digitalRead(OUT3)&&out_d[3]) {
    digitalWrite(OUT3, out_d[3]);
    delay(100);
  }
  digitalWrite(OUT3, out_d[3]);
  
  if(count_t >= (int)(1000.0/freq)){
    last_t=millis();
    analogWrite(OUT0, PWM_out[0] *out_d[0]);
    analogWrite(OUT1, PWM_out[1] *out_d[1]);
    analogWrite(OUT2, PWM_out[2] *out_d[2]);
    digitalWrite(AUXILIAR_LED, HIGH);
  }
  else if(blinking){ //OFF In case of blinking mode
    analogWrite(OUT0, 0);
    analogWrite(OUT1, 0);
    analogWrite(OUT2, 0);
    digitalWrite(AUXILIAR_LED, LOW);
  }

  count_t = millis()-last_t;
  
}
