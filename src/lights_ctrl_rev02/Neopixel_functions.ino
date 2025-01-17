/*void neopixel_setpixel() {
  String arguments = "Comando(s) aceito(s): \n";
  int R, G, B;
  for (uint8_t i = 0; i < server.args(); i++) {
    R = server.arg(i).substring(0, server.arg(i).indexOf(',')).toInt() % 256;
    G = server.arg(i).substring(server.arg(i).indexOf(',') + 1, server.arg(i).lastIndexOf(',')).toInt() % 256;
    B = server.arg(i).substring(server.arg(i).lastIndexOf(',') + 1).toInt() % 256;
    pixels->setPixelColor(
      server.argName(i).toInt(),  //led a ser alterado
      pixels->Color(R, G, B)     //cor
    );
    arguments += String(i) + ") Led " + server.argName(i) + " = (" + server.arg(i) + ")\n";
  }

  Serial.println(arguments);
  server.send(404, "text/plain", arguments);

  pixels->show();
}
*/
void neopixel_setpixelRange(AsyncWebServerRequest *request) {
  String arguments = "Argumento(s) aceito(s): \n";
  int R, G, B, W;
  int StartLed, EndLed;
  for (uint8_t i = 0; i < request->args(); i++) {
    String argument = request->arg(i);
    R = argument.substring(0, argument.indexOf(',')).toInt() % 256;
    G = argument.substring(argument.indexOf(',') + 1, argument.indexOf(',', argument.indexOf(',') + 1)).toInt() % 256;
    B = argument.substring(argument.indexOf(',', argument.indexOf(',') + 1) + 1, argument.lastIndexOf(',')).toInt() % 256;
    W = argument.substring(argument.lastIndexOf(',') + 1).toInt() % 256;

    Serial.print(R);
    Serial.print(',');
    Serial.print(G);
    Serial.print(',');
    Serial.print(B);
    Serial.print(',');
    Serial.println(W);
    StartLed = request->argName(i).substring(0, request->argName(i).indexOf(',')).toInt();
    EndLed = request->argName(i).substring(request->argName(i).indexOf(',') + 1).toInt();

    arguments += String(i) + ") ";
    if (StartLed >= 0 && StartLed < numPixels && EndLed >= 0 && EndLed < numPixels && EndLed >= StartLed) {
      for (int j = StartLed; j <= EndLed; j++)
        pixels->setPixelColor(j, pixels->Color(R, G, B,W));
      arguments += "Leds de " + String(StartLed) + " a " + String(EndLed) + " = (" + request->arg(i) + ")";
    }
    else
      arguments += "Sintaxe incorreta";
    arguments += "\n";
  }
  request->send(404, "text/plain", arguments);
  pixels->show();
}

/*
void neopixel_pixelAnimacao() {
  String arguments = "Argumento(s) aceito(s) \n";
  String R, G, B, W;
  int StartLed, EndLed, delayNext;
  for (uint8_t i = 0; i < server.args(); i++) {//ex: ?0,10=C:100,30,40,D:100
    int indexCor = server.arg(i).indexOf("C:");
    int indexR = indexCor + 2;
    int indexG = server.arg(i).indexOf(',', indexR) + 1;
    int indexB = server.arg(i).indexOf(',', indexG) + 1;
    int indexW = server.arg(i).indexOf(',', indexB) + 1;

    delayNext = server.arg(i).substring(server.arg(i).indexOf("D:")+2).toInt();
    StartLed = server.argName(i).substring(0, server.argName(i).indexOf(',')).toInt();
    EndLed = server.argName(i).substring(server.argName(i).indexOf(',') + 1).toInt();
    
    filaLeds += String(delayNext)+"."+server.arg(i).substring(indexR, server.arg(i).indexOf(',', indexW))+","+String(StartLed)+","+String(EndLed)+",";
    arguments += String(i) + ") ";
    if (StartLed >= 0 && StartLed < numPixels && EndLed >= 0 && EndLed < numPixels) {
      arguments += "Leds de " + String(StartLed) + " a " + String(EndLed) + " = (" + server.arg(i) + ")";
    }
    else
      arguments += "Sintaxe incorreta";
    arguments += "\n";
  }
  filaLeds+="FIM";
  Serial.println(arguments);
  Serial.println(filaLeds);
  server.send(404, "text/plain", arguments);
}*/
