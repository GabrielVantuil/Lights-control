void toggleNpWhite(uint8_t value){
  digitalWrite(OUT3, out_d[3]);
  if(out_d[3]){
      delay(100);
      pixels->fill(pixels->Color(0,0,0, value),0,NUMPIXELS);
      pixels->show();
  }
}
void npAllPixels(){
  out_d[3] = !((RGBW[0] + RGBW[1] + RGBW[2] + RGBW[3]) == 0); //if any color is on, turn neopixel's power on
  digitalWrite(OUT3, out_d[3]);
  if(!out_d[3]) return;
  delay(100);
  pixels->fill(pixels->Color(RGBW[0], RGBW[1], RGBW[2], RGBW[3]),0,NUMPIXELS);
  pixels->show();
}
void neopixel_setPixels() {
  String arguments = "Argumento(s) aceito(s) \n";
  String R, G, B, W;
  int StartLed, EndLed, delayNext;
  bool isLoop = false;
  ledQueue = "";
  shiftQueue = "";
  loopQueue = "";
  for (uint8_t i = 0; i < server.args(); i++) {//ex: ?0,10=C:100,30,40,10,D:100   //from 0 to 10 with RGBW = 100,30,40,10 and 100ms delay between each led
    if(server.argName(i)=="loop"){
      isLoop = true;
      loopQueue.remove(0);
      ledQueue += "L";
      arguments += String(i) + ") " + "loop\n";
      continue;
    }
    if(server.argName(i)=="D"){
      if(isLoop) loopQueue += "d" + server.arg(i)+",";
      else  ledQueue += "d" + server.arg(i)+",";
      arguments += String(i) + ") " + "delay("+ server.arg(i) + ")\n";
      continue;
    }
    if(server.argName(i)=="shift"){
      shiftQueue.remove(0);
      ledQueue += "S"+server.arg(i)+";";
      arguments += String(i) + ") " + "shift\n";
      continue;
    }
    if(server.argName(i)=="endshift" || server.argName(i)=="endShift"){
      ledQueue += "END";
      arguments += String(i) + ") " + "endshift\n";
      continue;
    }
    int indexR = 0;
    int indexG = server.arg(i).indexOf(',', indexR) + 1;
    int indexB = server.arg(i).indexOf(',', indexG) + 1;
    int indexW = server.arg(i).indexOf(',', indexB) + 1;
    if(server.arg(i).indexOf("D:")!=-1){
      delayNext = server.arg(i).substring(server.arg(i).indexOf("D:")+2).toInt();
    }else delayNext = 0;
    
    StartLed = server.argName(i).substring(0, server.argName(i).indexOf(',')).toInt();
    EndLed = server.argName(i).substring(server.argName(i).indexOf(',') + 1).toInt();

    if(isLoop){
      loopQueue += String(delayNext)+"."+server.arg(i).substring(indexR, server.arg(i).indexOf(',', indexW))+"f"+String(StartLed)+"t"+String(EndLed)+";";      
    }
    else{
      ledQueue += String(delayNext)+"."+server.arg(i).substring(indexR, server.arg(i).indexOf(',', indexW))+"f"+String(StartLed)+"t"+String(EndLed)+";";
    }
    arguments += String(i) + ") ";
    if (StartLed >= 0 && StartLed < (NUMPIXELS*2) && EndLed >= 0 && EndLed < (NUMPIXELS*2)) {
      arguments += "Leds from " + String(StartLed) + " to " + String(EndLed) + " = (" + server.arg(i) + ")";
    }
    else  arguments += "Wrong syntax";
    
    arguments += "\n";
  }
  if(!isLoop) ledQueue+="END";
  #if defined(ENABLE_LOG)
//  Serial.println(arguments);
//  Serial.print("ledQueue: ");
//  Serial.println(ledQueue);
//  Serial.print("shiftQueue: ");
//  Serial.println(shiftQueue);
//  Serial.println("------------------------------------------------");
  #endif

  out_d[3] = true;
  digitalWrite(OUT3, out_d[3]);
  delay(100);
  server.send(404, "text/plain", arguments);
}
String shiftNp(String original, int shift){
  String newStr = original;
  for(int i = 0; i<newStr.length();i++){
    if(newStr.charAt(i) == 'f'){
      int fromLed = (newStr.substring(i+1, newStr.indexOf('t',i)).toInt()+shift+NUMPIXELS);
      int toLed = (newStr.substring(newStr.indexOf('t',i)+1, newStr.indexOf(';',i)).toInt()+shift+NUMPIXELS);
      while((fromLed>(NUMPIXELS)) && (toLed>(NUMPIXELS))){
        fromLed -= NUMPIXELS;
        toLed -= NUMPIXELS;
      }
      newStr = newStr.substring(0,i+1) + String(fromLed) + "t" + String(toLed)+ newStr.substring(newStr.indexOf(';',i));
    }
  }
  return newStr;
}
