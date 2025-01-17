//
//void neopixel_pixelAnimacao() {
//  String arguments = "Argumento(s) aceito(s) \n";
//  int R, G, B;
//  int StartLed, EndLed, delayNext;
//  for (uint8_t i = 0; i < server.args(); i++) {//ex: ?0,10=C:100,30,40,D:100
//    int indexCor = server.arg(i).indexOf("C:");
//    int indexR = indexCor + 2;
//    int indexG = server.arg(i).indexOf(',', indexR) + 1;
//    int indexB = server.arg(i).indexOf(',', indexG) + 1;
//
//    R = server.arg(i).substring(indexR, indexG - 1).toInt() % 256;
//    G = server.arg(i).substring(indexG, indexB - 1).toInt() % 256;
//    B = server.arg(i).substring(indexB, server.arg(i).indexOf(',', indexB)).toInt() % 256;
//
//    delayNext = server.arg(i).substring(server.arg(i).indexOf("D:")+2).toInt();
//    StartLed = server.argName(i).substring(0, server.argName(i).indexOf(',')).toInt();
//    EndLed = server.argName(i).substring(server.argName(i).indexOf(',') + 1).toInt();
//
//    arguments += String(i) + ") ";
//    if (StartLed >= 0 && StartLed < numPixels && EndLed >= 0 && EndLed < numPixels) {
//      for (int j = StartLed; j != EndLed+abs(EndLed - StartLed)/(EndLed - StartLed); j+= abs(EndLed - StartLed)/(EndLed - StartLed)) {//faz em qualquer direção até igualar atual e final
//        pixels->setPixelColor(j, pixels->Color(R, G, B));
//        long start = millis();
//        while(millis()<start+delayNext);
//        pixels->show();
//      }
//      arguments += "Leds de " + String(StartLed) + " a " + String(EndLed) + " = (" + server.arg(i) + ")";
//    }
//    else
//      arguments += "Sintaxe incorreta";
//    arguments += "\n";
//  }
//  Serial.println(arguments);
//  server.send(404, "text/plain", arguments);
//}

//
//void neopixel_pixelAnimacao2() {
//  String arguments = "Argumento(s) aceito(s) \n";
//  String R, G, B;
//  int StartLed, EndLed, delayNext;
//  for (uint8_t i = 0; i < server.args(); i++) {//ex: ?0,10=C:100,30,40,D:100
//    int indexCor = server.arg(i).indexOf("C:");
//    int indexR = indexCor + 2;
//    int indexG = server.arg(i).indexOf(',', indexR) + 1;
//    int indexB = server.arg(i).indexOf(',', indexG) + 1;
//
//    delayNext = server.arg(i).substring(server.arg(i).indexOf("D:")+2).toInt();
//    StartLed = server.argName(i).substring(0, server.argName(i).indexOf(',')).toInt();
//    EndLed = server.argName(i).substring(server.argName(i).indexOf(',') + 1).toInt();
//    
//    filaLeds += "Novo" + String(delayNext)+"."+server.arg(i).substring(indexR, server.arg(i).indexOf(',', indexB))+",";
//    arguments += String(i) + ") ";
//    if (StartLed >= 0 && StartLed < numPixels && EndLed >= 0 && EndLed < numPixels) {
//      for (int j = StartLed; j != EndLed+abs(EndLed - StartLed)/(EndLed - StartLed); j+= abs(EndLed - StartLed)/(EndLed - StartLed)) {//faz em qualquer direção até igualar atual e final
//        filaLeds +=String((char)(j+48));
//      }
//      arguments += "Leds de " + String(StartLed) + " a " + String(EndLed) + " = (" + server.arg(i) + ")";
//    }
//    else
//      arguments += "Sintaxe incorreta";
//    arguments += "\n";
//  }
//  Serial.println(arguments);
//  Serial.println(filaLeds);
//  server.send(404, "text/plain", arguments);
//}
//void loop(void) {
//  server.handleClient();
//
//  if(filaLeds.length()!=0){
//    if(filaLeds.substring(0,4)=="Novo"){
//      delayNext = filaLeds.substring(4,filaLeds.indexOf('.')+1).toInt();
//      filaLeds.remove(0,filaLeds.indexOf('.')+1);
//      filaR=filaLeds.substring(0,filaLeds.indexOf(',')+1).toInt();
//      filaLeds.remove(0,filaLeds.indexOf(',')+1);
//      filaG=filaLeds.substring(0,filaLeds.indexOf(',')+1).toInt();
//      filaLeds.remove(0,filaLeds.indexOf(',')+1);
//      filaB=filaLeds.substring(0,filaLeds.indexOf(',')+1).toInt();
//      filaLeds.remove(0,filaLeds.indexOf(',')+1);
//    }
//    pixels->setPixelColor((int)filaLeds.charAt(0)-48
//      , pixels->Color(filaR,filaG,filaB));
//    long start = millis();
//    while(millis()<start+delayNext);
//    pixels->show();
//    
//    filaLeds.remove(0,1);
//  }
//}
